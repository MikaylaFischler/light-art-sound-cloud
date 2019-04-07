#ifndef IO_H_
#define IO_H_

#include <Arduino.h>
// #include <inttypes.h>

#include "config.h"

// #define LED_MODE_SOMETHING

void io_change_mode(void);
void io_init_mode_control(void);
int io_get_brightness(void);

#endif
