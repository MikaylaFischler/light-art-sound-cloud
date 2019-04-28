#include "io.h"

uint8_t led_mode = 0;
uint8_t hysteresis = 0x00;

/**
 * @brief Initialize the mode button control
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
 * @brief Get the current mode to be running in
 * 
 * @return uint8_t The mode
 */
uint8_t io_get_mode(void) { return led_mode; }

/**
 * @brief Initialize status LED to off
 * 
 */
void io_init_stat_led(void) {
	// default to off
	analogWrite(S_LED_R, 255);
	analogWrite(S_LED_G, 255);
	analogWrite(S_LED_B, 255);
}

/**
 * @brief Set the Common Anode RGB status LED color
 * 
 * @param r Red component (0 - 255)
 * @param g Green component (0 - 255)
 * @param b Blue component (0 - 255)
 */
void __io_write_status_led(uint8_t r, uint8_t g, uint8_t b) {
	analogWrite(S_LED_R, 255 - r);
	analogWrite(S_LED_G, 255 - g);
	analogWrite(S_LED_B, 255 - b);
}

/**
 * @brief Set the status LED to a yellowish color to indicate system is starting up
 * 
 */
void io_led_write_starting(void) {
	__io_write_status_led(200, 150, 0);
}

/**
 * @brief Set the status LED to green to indicate system is running
 * 
 */
void io_led_write_active(void) {
	__io_write_status_led(0, 200, 50);
}

/**
 * @brief Set the status LED to blue to indicate state is changing
 * 
 */
void io_led_write_changing(void) {
	__io_write_status_led(0, 0, 150);
}
