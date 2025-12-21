#include "25hour.h"
#include <time.h>

void convert_to_25hour(time_t now, Chrono25Time *chrono_time) {
    struct tm midnight = *localtime(&now);
    midnight.tm_hour = 0;
    midnight.tm_min = 0;
    midnight.tm_sec = 0;

    time_t seconds_since_midnight = difftime(now, mktime(&midnight));
    time_t new_seconds_total = seconds_since_midnight * 25 / 24;

    chrono_time->new_hours = new_seconds_total / 3600;
    chrono_time->new_hours %= 25;
    chrono_time->new_minutes = (new_seconds_total / 60) % 60;
    chrono_time->new_seconds = new_seconds_total % 60;
}