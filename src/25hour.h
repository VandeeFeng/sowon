#ifndef _25HOUR_H_
#define _25HOUR_H_

#include <time.h>

typedef struct {
    int new_hours;
    int new_minutes;
    int new_seconds;
} Chrono25Time;

void convert_to_25hour(time_t now, Chrono25Time *chrono_time);

#endif
