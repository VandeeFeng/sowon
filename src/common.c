#include "digits.h"
#include "cat_guard.h"

#ifdef PENGER
#include "penger_walk_sheet.h"
#endif

#include "25hour.h"

#include <math.h>
#include <time.h>

#define FPS 60
#define COLON_INDEX 10
#define SPRITE_CHAR_WIDTH (300 / 2)
#define SPRITE_CHAR_HEIGHT (380 / 2)
#define WIGGLE_COUNT 3
#define WIGGLE_DURATION (0.40f / WIGGLE_COUNT)
#define DIGIT_WIDTH (300 / 2)
#define CHAR_WIDTH (300 / 2)
#define CHAR_HEIGHT (380 / 2)
#define CHARS_COUNT 8
#define TEXT_WIDTH (CHAR_WIDTH * CHARS_COUNT)
#define TEXT_HEIGHT (CHAR_HEIGHT)
#define MAIN_COLOR_R 220
#define MAIN_COLOR_G 220
#define MAIN_COLOR_B 220
#define PAUSE_COLOR_R 220
#define PAUSE_COLOR_G 120
#define PAUSE_COLOR_B 120
#define BACKGROUND_COLOR_R 24
#define BACKGROUND_COLOR_G 24
#define BACKGROUND_COLOR_B 24
#define PENGER_STEPS_PER_SECOND 3
#define PENGER_SCALE 4
#define SCALE_FACTOR 0.15f
#define TITLE_CAP 256
#define CAT_GUARD_FRAME_WIDTH 128
#define CAT_GUARD_FRAME_HEIGHT 48
#define CAT_GUARD_WIDTH 160
#define CAT_GUARD_HEIGHT 60
#define CAT_GUARD_MARGIN 12

typedef enum {
    MODE_ASCENDING = 0,
    MODE_COUNTDOWN,
    MODE_CLOCK,
    MODE_25HOUR_CLOCK,
} Mode;

float parse_time(const char *time)
{
    float result = 0.0f;

    while (*time) {
        char *endptr = NULL;
        float x = strtof(time, &endptr);

        if (time == endptr) {
            fprintf(stderr, "`%s` is not a number\n", time);
            exit(1);
        }

        switch (*endptr) {
        case '\0':
        case 's': result += x;                 break;
        case 'm': result += x * 60.0f;         break;
        case 'h': result += x * 60.0f * 60.0f; break;
        default:
            fprintf(stderr, "`%c` is an unknown time unit\n", *endptr);
            exit(1);
        }

        time = endptr;
        if (*time) time += 1;
    }

    return result;
}

typedef struct {
    bool active;
} CatGuard;

static bool keyboard_guard_active;

bool set_keyboard_enabled(bool enabled)
{
#ifdef __linux__
    const char *event_state = enabled ? "enabled" : "disabled";
    char command[1024];
    snprintf(command, sizeof(command),
             "command -v jq >/dev/null && command -v swaymsg >/dev/null && "
             "commands=$(swaymsg -t get_inputs -r | "
             "jq -er '[.[] | select(.type == \"keyboard\") | \"input \" + "
             "(.identifier | @json) + \" events %s\"] | "
             "if length > 0 then join(\"; \") else error(\"no keyboards\") end') && "
             "swaymsg \"$commands\" | jq -e 'all(.success)' >/dev/null",
             event_state);

    if (getenv("SWAYSOCK") == NULL || system(command) != 0) {
        fprintf(stderr, "Could not %s keyboards by ID with swaymsg.\n", event_state);
        return false;
    }
    return true;
#else
    (void) enabled;
    fprintf(stderr, "Cat Guard requires the Sway compositor.\n");
    return false;
#endif
}

void restore_keyboard_at_exit(void)
{
    if (keyboard_guard_active) set_keyboard_enabled(true);
}

void cat_guard_toggle(CatGuard *guard)
{
    bool active = !guard->active;
    if (!set_keyboard_enabled(!active)) {
        if (active) set_keyboard_enabled(true);
        return;
    }

    guard->active = active;
    keyboard_guard_active = active;
}

float cat_guard_scale(int window_width, int window_height)
{
    float width_scale = (float) window_width / TEXT_WIDTH;
    float height_scale = (float) window_height / (TEXT_HEIGHT * 2);
    float horizontal_limit = (float) window_width / (CAT_GUARD_WIDTH + CAT_GUARD_MARGIN * 2);
    float vertical_limit = (float) window_height / (CAT_GUARD_HEIGHT + CAT_GUARD_MARGIN * 2);
    float proportional_scale = sqrtf(width_scale * height_scale);
    return fminf(proportional_scale, fminf(horizontal_limit, vertical_limit));
}

RGFW_rect cat_guard_rect(int window_width, int window_height)
{
    float scale = cat_guard_scale(window_width, window_height);
    int width = (int) floorf(CAT_GUARD_WIDTH * scale);
    int height = (int) floorf(CAT_GUARD_HEIGHT * scale);
    int margin = (int) floorf(CAT_GUARD_MARGIN * scale);
    RGFW_rect rect = {
        window_width - width - margin,
        margin,
        width,
        height
    };
    return rect;
}

bool point_in_rect(int x, int y, RGFW_rect rect)
{
    return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
}

typedef struct {
    Mode mode;
    float displayed_time;
    int paused;
    int exit_after_countdown;

    int quit;
    size_t wiggle_index;
    float wiggle_cooldown;
    float user_scale;
    char prev_title[TITLE_CAP];
} State;

void parse_state_from_args(State *state, int argc, char **argv)
{
    memset(state, 0, sizeof(*state));

    state->wiggle_cooldown = WIGGLE_DURATION;
    state->user_scale = 1.0f;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-p") == 0) {
            state->paused = 1;
        } else if (strcmp(argv[i], "-e") == 0) {
            state->exit_after_countdown = 1;
        } else if (strcmp(argv[i], "clock") == 0) {
            state->mode = MODE_CLOCK;
        } else if (strcmp(argv[i], "25hour") == 0 || strcmp(argv[i], "chrono25") == 0) {
            state->mode = MODE_25HOUR_CLOCK;
        } else {
            state->mode = MODE_COUNTDOWN;
            state->displayed_time = parse_time(argv[i]);
        }
    }
}

void state_update(State *state, float dt)
{
    if (state->wiggle_cooldown <= 0.0f) {
        state->wiggle_index++;
        state->wiggle_cooldown = WIGGLE_DURATION;
    }
    state->wiggle_cooldown -= dt;

    if (!state->paused) {
        switch (state->mode) {
        case MODE_ASCENDING: {
            // TODOOOOO: display_time should not depend on `dt` AT ALL!
            //
            // Capture some sort of timestamp from the start of the application, and depending on the mode
            // display the time relative to the start accordingly. That way the timer is alway accurate
            // regardless of the FPS.
            //
            // Maybe even wiggle animation should not depend on the `dt`.
            state->displayed_time += dt;
        } break;
        case MODE_COUNTDOWN: {
            if (state->displayed_time > 1e-6) {
                state->displayed_time -= dt;
            } else {
                state->displayed_time = 0.0f;
                if (state->exit_after_countdown) {
                    exit(0);
                }
            }
        } break;
        case MODE_CLOCK: {
            float displayed_time_prev = state->displayed_time;
            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            state->displayed_time = tm->tm_sec + tm->tm_min  * 60.0f + tm->tm_hour * 60.0f * 60.0f;
            if (state->displayed_time <= displayed_time_prev) {
                // same second, keep previous count and add subsecond resolution for penger
                if (floorf(displayed_time_prev) == floorf(displayed_time_prev+dt)) { // check for no newsecond shenaningans from dt
                    state->displayed_time = displayed_time_prev + dt;
                } else {
                    state->displayed_time = displayed_time_prev;
                }
            }
        } break;
        case MODE_25HOUR_CLOCK: {
            time_t t = time(NULL);
            Chrono25Time chrono_time;
            convert_to_25hour(t, &chrono_time);
            state->displayed_time = chrono_time.new_seconds + chrono_time.new_minutes  * 60.0f + chrono_time.new_hours * 60.0f * 60.0f;
        } break;
        }
    }
}

void initial_pen(int w, int h, int *pen_x, int *pen_y, float user_scale, float *fit_scale)
{
    float text_aspect_ratio = (float) TEXT_WIDTH / (float) TEXT_HEIGHT;
    float window_aspect_ratio = (float) w / (float) h;
    if(text_aspect_ratio > window_aspect_ratio) {
        *fit_scale = (float) w / (float) TEXT_WIDTH;
    } else {
        *fit_scale = (float) h / (float) TEXT_HEIGHT;
    }

    const int effective_digit_width = (int) floorf((float) DIGIT_WIDTH * user_scale * *fit_scale);
    const int effective_digit_height = (int) floorf((float) CHAR_HEIGHT * user_scale * *fit_scale);
    *pen_x = w / 2 - effective_digit_width * CHARS_COUNT / 2;
    *pen_y = h / 2 - effective_digit_height / 2;
}
