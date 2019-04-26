#ifndef IO_H_
#define IO_H_

#include <Arduino.h>
#include <inttypes.h>

IntervalTimer shift_timer;

#include "config.h"

#define LED_NUM_MODES 3

#define LED_MODE_BASS_SIDE_PULSE 0
#define LED_MODE_BASS_RANGE_PULSE 1
#define LED_MODE_BASS_TREBLE_PULSE 2

void io_init_mode_control(void);

void __io_mode_shift(void);
void io_change_mode(void);

int io_get_brightness(void);

#endif
