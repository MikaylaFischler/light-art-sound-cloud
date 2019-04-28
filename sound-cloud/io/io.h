#ifndef IO_H_
#define IO_H_

#include <Arduino.h>
#include <inttypes.h>

IntervalTimer shift_timer;

#include "config.h"

#define LED_NUM_MODES 5

#define LED_MODE_BASS_SIDE_PULSE 0
#define LED_MODE_BASS_RANGE_PULSE 1
#define LED_MODE_BASS_TREBLE_PULSE 2
#define LED_MODE_LINEAR_VISUALIZER_RAINBOW 3
#define LED_MODE_LINEAR_VISUALIZER_FIRE 4

void io_init_mode_control(void);
void __io_mode_shift(void);
void io_change_mode(void);

void io_init_stat_led(void);
void __io_write_status_led(uint8_t r, uint8_t g, uint8_t b);
void io_led_write_active(void);
void io_led_write_changing(void);

#endif
