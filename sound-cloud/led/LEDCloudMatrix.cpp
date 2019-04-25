#include "LEDCloudMatrix.hpp"

/**
 * @brief Produces Red component of 32-bit RGB
 * 
 * @param color 32-bit RGB color
 * @return uint8_t Red component (8-bit) of a 32-bit color
 */
uint8_t LEDCloudMatrix::redFromColor(uint32_t color) {
    return (color >> 16) & 0xFF;
}

/**
 * @brief Produces Green component of 32-bit RGB
 * 
 * @param color 32-bit RGB color
 * @return uint8_t Green component (8-bit) of a 32-bit color
 */
uint8_t LEDCloudMatrix::greenFromColor(uint32_t color) {
    return (color >> 8) & 0xFF;
}

/**
 * @brief Produces Blue component of 32-bit RGB
 * 
 * @param color 32-bit RGB color
 * @return uint8_t Blue component (8-bit) of a 32-bit color
 */
uint8_t LEDCloudMatrix::blueFromColor(uint32_t color) {
    return color & 0xFF;
}

/**
 * @brief Produce 32-bit RGB from 8-bit components
 * 
 * @param r Red component
 * @param g Green component
 * @param b Blue component
 * @return uint8_t 32-bit RGB color
 */
uint32_t LEDCloudMatrix::color(uint8_t r, uint8_t g, uint8_t b) {
	return ((uint32_t) r << 16) | ((uint32_t) g <<  8) | b;
}

/**
 * @brief Continuous color wheel function. Input a value 0 to 255 to get a color value. 
 * The colors are a transition r - g - b - back to r.
 * 
 * @param wheel_pos A number from 0 to 255 giving a position in the color wheel
 * @return uint32_t 
 */
uint32_t LEDCloudMatrix::colorWheel(uint8_t wheel_pos) {
	wheel_pos = 255 - wheel_pos;

	if (wheel_pos < 85) {
		return LEDCloudMatrix::color(255 - wheel_pos * 3, 0, wheel_pos * 3);
	}

	if (wheel_pos < 170) {
		wheel_pos -= 85;
		return LEDCloudMatrix::color(0, wheel_pos * 3, 255 - wheel_pos * 3);
	}

	wheel_pos -= 170;
	return LEDCloudMatrix::color(wheel_pos * 3, 255 - wheel_pos * 3, 0);
}
