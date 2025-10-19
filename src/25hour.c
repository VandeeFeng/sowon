#include "25hour.h"
#include <time.h>

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
}