#include <stdio.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    int new_hours;
    int new_minutes;
    int new_seconds;
} Chrono25Time;

const char *const ascii_digits[10][3] = {
    {" _ ", "| |", "|_|"}, // 0
    {"   ", "  |", "  |"}, // 1
    {" _ ", " _|", "|_ "}, // 2
    {" _ ", " _|", " _|"}, // 3
    {"   ", "|_|", "  |"}, // 4
    {" _ ", "|_ ", " _|"}, // 5
    {" _ ", "|_ ", "|_|"}, // 6
    {" _ ", "  |", "  |"}, // 7
    {" _ ", "|_|", "|_|"}, // 8
    {" _ ", "|_|", " _|"}  // 9
};

const char *const ascii_colon[3] = {
    "   ",
    " . ",
    "   "
};

void convert_to_25hour(time_t now, Chrono25Time *chrono_time) {
    struct tm midnight = *localtime(&now);
    midnight.tm_hour = 0;
    midnight.tm_min = 0;
    midnight.tm_sec = 0;

    double seconds_since_midnight = difftime(now, mktime(&midnight));
    double new_seconds = seconds_since_midnight * 25.0 / 24.0;

    int chrono_hours = (int)(new_seconds / 3600) % 25;
    int chrono_minutes = (int)(new_seconds / 60) % 60;
    int chrono_seconds = (int)new_seconds % 60;

    chrono_time->new_hours = chrono_hours;
    chrono_time->new_minutes = chrono_minutes;
    chrono_time->new_seconds = chrono_seconds;

    struct tm *real_time = localtime(&now);

    printf("\033[2J\033[H");

    printf("25Hour-Time:\n");

    // the ASCII clock
    for (int i = 0; i < 3; i++) {
        printf("%s%s", ascii_digits[chrono_hours / 10][i], ascii_digits[chrono_hours % 10][i]);
        printf("%s", ascii_colon[i]);
        printf("%s%s", ascii_digits[chrono_minutes / 10][i], ascii_digits[chrono_minutes % 10][i]);
        printf("%s", ascii_colon[i]);
        printf("%s%s\n", ascii_digits[chrono_seconds / 10][i], ascii_digits[chrono_seconds % 10][i]);
    }

    printf("\nReal-Time: [%02d:%02d:%02d]\n",
           real_time->tm_hour, real_time->tm_min, real_time->tm_sec);

    fflush(stdout);
}

int main() {
    Chrono25Time chrono_time;

    while (1) {
        time_t now = time(NULL);
        convert_to_25hour(now, &chrono_time);
        sleep(1);
    }
    return 0;
}
