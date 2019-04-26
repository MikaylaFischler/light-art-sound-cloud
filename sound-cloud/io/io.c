#include "io.h"

uint8_t led_mode = 0;
uint8_t hysteresis = 0x00;

/**
 * @brief 
 * 
 */
void io_init_mode_control(void) {
	pinMode(LED_MODE_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(LED_MODE_PIN), io_change_mode, FALLING);

	shift_timer.begin(__io_mode_shift, 1000); 
}

/**
 * @brief Shift the hysteresis varaible to add a software debounce to the existing hardware debounce
 * 
 */
void __io_mode_shift(void) { hysteresis <<= 1; }

/**
 * @brief Update the LED mode when the button is pressed, taking into account the last 8000us of readings to software debounce the input
 * 
 */
void io_change_mode(void) {
	if (!hysteresis) {
		if (++led_mode == LED_NUM_MODES) { led_mode = 0; }
		hysteresis |= 0x1;
	}
}

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t io_get_mode(void) { return led_mode; }

/**
 * @brief 
 * 
 * @return int 
 */
int io_get_brightness(void) { return analogRead(LED_BRIGHTNESS_PIN); }
