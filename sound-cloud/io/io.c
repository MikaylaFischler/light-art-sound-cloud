#include "io.h"

uint8_t led_mode = 0;

void io_init_mode_control(void) {
	attachInterrupt(LED_MODE_PIN, io_change_mode, RISING);
}

void io_change_mode(void) {
	// change LED mode
}

uint8_t io_get_mode(void) { return led_mode; }

int io_get_brightness(void) { return analogRead(LED_BRIGHTNESS_PIN); }
