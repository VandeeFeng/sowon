#include <stdio.h>
#include <stdbool.h>
#define RGFW_IMPLEMENTATION
#include "RGFW.h"
#define GL_GLEXT_PROTOTYPES
#include <GL/glext.h>

#include "digits.h"
#include "penger_walk_sheet.h"

#include "common.c"
#include "glextloader.c"

const char *vert_shader =
    "#version 330\n"
    "precision mediump float;\n"
    "uniform vec2 scr_size;\n"
    "uniform vec4 dst_rect;\n"
    "out vec2 uv;\n"
    "void main(void)\n"
    "{\n"
    "    uv.x = (gl_VertexID & 1);\n"
    "    uv.y = ((gl_VertexID >> 1) & 1);\n"
    "    vec2 p = (dst_rect.xy + dst_rect.zw*uv)/scr_size;\n"
    "    p.y = 1.0 - p.y;\n"
    "    gl_Position = vec4(p * 2.0 - 1.0, 0.0, 1.0);\n"
    "}\n";

const char *frag_shader =
    "#version 330\n"
    "precision mediump float;\n"
    "uniform sampler2D tex;\n"
    "uniform vec2 tex_size;\n"
    "uniform vec4 src_rect;\n"
    "uniform vec4 color_mod;\n"
    "in vec2 uv;\n"
    "out vec4 out_color;\n"
    "void main(void) {\n"
    "    vec2 coord = (src_rect.xy + src_rect.zw*uv)/tex_size;\n"
    "    out_color = texture(tex, coord)*color_mod;\n"
    "}\n";

const char *shader_type_as_cstr(GLuint shader)
{
    switch (shader) {
    case GL_VERTEX_SHADER:
        return "GL_VERTEX_SHADER";
    case GL_FRAGMENT_SHADER:
        return "GL_FRAGMENT_SHADER";
    default:
        return "(Unknown)";
    }
}

bool compile_shader_source(const GLchar *source, GLenum shader_type, GLuint *shader)
{
    *shader = glCreateShader(shader_type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

    GLint compiled = 0;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLchar message[1024];
        GLsizei message_size = 0;
        glGetShaderInfoLog(*shader, sizeof(message), &message_size, message);
        fprintf(stderr, "ERROR: could not compile %s\n", shader_type_as_cstr(shader_type));
        fprintf(stderr, "%.*s\n", message_size, message);
        return false;
    }

    return true;
}

bool link_program(GLuint vert_shader, GLuint frag_shader, GLuint *program)
{
    *program = glCreateProgram();

    glAttachShader(*program, vert_shader);
    glAttachShader(*program, frag_shader);
    glLinkProgram(*program);

    GLint linked = 0;
    glGetProgramiv(*program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLsizei message_size = 0;
        GLchar message[1024];

        glGetProgramInfoLog(*program, sizeof(message), &message_size, message);
        fprintf(stderr, "Program Linking: %.*s\n", message_size, message);
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    return program;
}

GLuint load_png_file_data_as_gl_texture(uint32_t *data, size_t width, size_t height)
{
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 width,
                 height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 data);
    return texture;
}

GLint tex_uni;
GLint dst_rect_uni;
GLint scr_size_uni;
GLint src_rect_uni;
GLint tex_size_uni;
GLint color_mod_uni;

void set_texture_color_mod(GLfloat r, GLfloat g, GLfloat b)
{
    glUniform4f(color_mod_uni, r, g, b, 1);
}

void texture_copy(GLint texture_unit, int tex_width, int tex_height, RGFW_rect src_rect, RGFW_rect dst_rect)
{
    glUniform1i(tex_uni, texture_unit);
    glUniform4f(dst_rect_uni, dst_rect.x, dst_rect.y, dst_rect.w, dst_rect.h);
    glUniform4f(src_rect_uni, src_rect.x, src_rect.y, src_rect.w, src_rect.h);
    glUniform2f(tex_size_uni, tex_width, tex_height);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void render_digit_at(size_t digit_index, size_t wiggle_index, int *pen_x, int *pen_y, float user_scale, float fit_scale)
{
    const int effective_digit_width = (int) floorf((float) CHAR_WIDTH * user_scale * fit_scale);
    const int effective_digit_height = (int) floorf((float) CHAR_HEIGHT * user_scale * fit_scale);

    const RGFW_rect src_rect = {
        (int) (digit_index * SPRITE_CHAR_WIDTH),
        (int) (wiggle_index * SPRITE_CHAR_HEIGHT),
        SPRITE_CHAR_WIDTH,
        SPRITE_CHAR_HEIGHT
    };
    const RGFW_rect dst_rect = {
        *pen_x,
        *pen_y,
        effective_digit_width,
        effective_digit_height
    };

    texture_copy(0, digits_width, digits_height, src_rect, dst_rect);

    *pen_x += effective_digit_width;
}

#ifdef PENGER
void render_penger_at(int window_width, int window_height, float time, int flipped)
{
    int sps  = PENGER_STEPS_PER_SECOND;

    int step = (int)(time*sps)%(60*sps); //step index [0,60*sps-1]

    float progress  = step/(60.0*sps); // [0,1]

    int frame_index = step%2;

    float penger_drawn_width = ((float)penger_width / 2) / PENGER_SCALE;

    float penger_walk_width = window_width + penger_drawn_width;

    RGFW_rect src_rect = {
        (int) (penger_width / 2) * frame_index,
        0,
        (int) penger_width / 2,
        (int) penger_height
    };

    RGFW_rect dst_rect = {
        floorf((float)penger_walk_width * progress - penger_drawn_width),
        window_height - (penger_height / PENGER_SCALE),
        (int) (penger_width / 2) / PENGER_SCALE,
        (int) penger_height / PENGER_SCALE
    };

    if (flipped) {
        src_rect.x += src_rect.w;
        src_rect.w *= -1;
    }
    texture_copy(1, penger_width, penger_height, src_rect, dst_rect);
}
#endif

int main(int argc, char **argv)
{
    State state = {0};

    parse_state_from_args(&state, argc, argv);

    RGFW_setGLHint(RGFW_glProfile, RGFW_glCore);
    RGFW_setGLHint(RGFW_glMajor, 3);
    RGFW_setGLHint(RGFW_glMinor, 3);

    RGFW_rect win_rect = RGFW_RECT(0, 0, TEXT_WIDTH, TEXT_HEIGHT*2);
    RGFW_window* win = RGFW_createWindow("sowon", win_rect, (u64)0);

    if (!load_gl_extensions()) return 1;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint vert_shader_id;
    if (!compile_shader_source(vert_shader, GL_VERTEX_SHADER, &vert_shader_id)) return 1;
    GLuint frag_shader_id;
    if (!compile_shader_source(frag_shader, GL_FRAGMENT_SHADER, &frag_shader_id)) return 1;
    GLuint program;
    if (!link_program(vert_shader_id, frag_shader_id, &program)) return 1;
    glUseProgram(program);

    tex_uni       = glGetUniformLocation(program, "tex");
    dst_rect_uni  = glGetUniformLocation(program, "dst_rect");
    scr_size_uni  = glGetUniformLocation(program, "scr_size");
    src_rect_uni  = glGetUniformLocation(program, "src_rect");
    tex_size_uni  = glGetUniformLocation(program, "tex_size");
    color_mod_uni = glGetUniformLocation(program, "color_mod");

    glUniform4f(dst_rect_uni, 0, 0, 500, 500);
    glUniform2f(scr_size_uni, win_rect.w, win_rect.h);
    glUniform4f(src_rect_uni, 0, 0, CHAR_WIDTH, CHAR_HEIGHT);
    glUniform2f(tex_size_uni, digits_width, digits_height);

    glActiveTexture(GL_TEXTURE0);
    load_png_file_data_as_gl_texture(digits_data, digits_width, digits_height);
    glActiveTexture(GL_TEXTURE1);
    load_png_file_data_as_gl_texture(penger_data, penger_width, penger_height);

    set_texture_color_mod(MAIN_COLOR_R/255.0f, MAIN_COLOR_G/255.0f, MAIN_COLOR_B/255.0f);
    if (state.paused) {
        set_texture_color_mod(PAUSE_COLOR_R/255.0f, PAUSE_COLOR_G/255.0f, PAUSE_COLOR_B/255.0f);
    } else {
        set_texture_color_mod(MAIN_COLOR_R/255.0f, MAIN_COLOR_G/255.0f, MAIN_COLOR_B/255.0f);
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    while (RGFW_window_shouldClose(win) == RGFW_FALSE) {
        // INPUT BEGIN //////////////////////////////
        while (RGFW_window_checkEvent(win)) {
            switch (win->event.type) {
            case RGFW_windowResized: {
                glViewport(0, 0, win->r.w, win->r.h);
                glUniform2f(scr_size_uni, win->r.w, win->r.h);
            } break;
            case RGFW_keyPressed: {
                switch (win->event.key) {
                case RGFW_space: {
                    state.paused = !state.paused;
                    if (state.paused) {
                        set_texture_color_mod(PAUSE_COLOR_R/255.0f, PAUSE_COLOR_G/255.0f, PAUSE_COLOR_B/255.0f);
                    } else {
                        set_texture_color_mod(MAIN_COLOR_R/255.0f, MAIN_COLOR_G/255.0f, MAIN_COLOR_B/255.0f);
                    }
                } break;

                // case SDLK_KP_PLUS:
                case RGFW_equals: {
                    state.user_scale += SCALE_FACTOR * state.user_scale;
                } break;

                // case SDLK_KP_MINUS:
                case RGFW_minus: {
                    state.user_scale -= SCALE_FACTOR * state.user_scale;
                } break;

                // case SDLK_KP_0:
                case RGFW_0: {
                    state.user_scale = 1.0f;
                } break;

                case RGFW_F5: {
                    state.displayed_time = 0.0f;
                    state.paused = 0;
                    for (int i = 1; i < argc; ++i) {
                        if (strcmp(argv[i], "-p") == 0) {
                            state.paused = 1;
                        } else {
                            state.displayed_time = parse_time(argv[i]);
                        }
                    }
                    if (state.paused) {
                        set_texture_color_mod(PAUSE_COLOR_R/255.0f, PAUSE_COLOR_G/255.0f, PAUSE_COLOR_B/255.0f);
                    } else {
                        set_texture_color_mod(MAIN_COLOR_R/255.0f, MAIN_COLOR_G/255.0f, MAIN_COLOR_B/255.0f);
                    }
                } break;

                // case RGFW_F11: {
                //     Uint32 window_flags;
                //     secc(window_flags = SDL_GetWindowFlags(window));
                //     if(window_flags & SDL_WINDOW_FULLSCREEN_DESKTOP) {
                //         secc(SDL_SetWindowFullscreen(window, 0));
                //     } else {
                //         secc(SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP));
                //     }
                // } break;
                }
            } break;
            }
        }
        // INPUT END //////////////////////////////

        // RENDER BEGIN //////////////////////////////
        glClearColor(BACKGROUND_COLOR_R/255.0f, BACKGROUND_COLOR_G/255.0f, BACKGROUND_COLOR_B/255.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        {
            const size_t t = (size_t) floorf(fmaxf(state.displayed_time, 0.0f));
            // PENGER BEGIN //////////////////////////////

            #ifdef PENGER
                render_penger_at(win->r.w, win->r.h, state.displayed_time, state.mode==MODE_COUNTDOWN);
            #endif

            // PENGER END //////////////////////////////

            // DIGITS BEGIN //////////////////////////////
            int pen_x, pen_y;
            float fit_scale = 1.0;
            initial_pen(win->r.w, win->r.h, &pen_x, &pen_y, state.user_scale, &fit_scale);

            // TODO: support amount of hours >99
            const size_t hours = t / 60 / 60;
            render_digit_at(hours / 10,   state.wiggle_index      % WIGGLE_COUNT, &pen_x, &pen_y, state.user_scale, fit_scale);
            render_digit_at(hours % 10,  (state.wiggle_index + 1) % WIGGLE_COUNT, &pen_x, &pen_y, state.user_scale, fit_scale);
            render_digit_at(COLON_INDEX,  state.wiggle_index      % WIGGLE_COUNT, &pen_x, &pen_y, state.user_scale, fit_scale);

            const size_t minutes = t / 60 % 60;
            render_digit_at(minutes / 10, (state.wiggle_index + 2) % WIGGLE_COUNT, &pen_x, &pen_y, state.user_scale, fit_scale);
            render_digit_at(minutes % 10, (state.wiggle_index + 3) % WIGGLE_COUNT, &pen_x, &pen_y, state.user_scale, fit_scale);
            render_digit_at(COLON_INDEX,  (state.wiggle_index + 1) % WIGGLE_COUNT, &pen_x, &pen_y, state.user_scale, fit_scale);

            const size_t seconds = t % 60;
            render_digit_at(seconds / 10, (state.wiggle_index + 4) % WIGGLE_COUNT, &pen_x, &pen_y, state.user_scale, fit_scale);
            render_digit_at(seconds % 10, (state.wiggle_index + 5) % WIGGLE_COUNT, &pen_x, &pen_y, state.user_scale, fit_scale);

            char title[TITLE_CAP];
            snprintf(title, sizeof(title), "%02zu:%02zu:%02zu - sowon", hours, minutes, seconds);
            if (strcmp(state.prev_title, title) != 0) {
                RGFW_window_setName(win, title);
            }
            memcpy(title, state.prev_title, TITLE_CAP);
            // DIGITS END //////////////////////////////
        }

        RGFW_window_swapBuffers(win);
        // RENDER END //////////////////////////////

        // UPDATE BEGIN //////////////////////////////
        state_update(&state, 0.016/*fps_dt.dt*/);
        // UPDATE END //////////////////////////////
    }

    RGFW_window_close(win);

    return 0;
}
