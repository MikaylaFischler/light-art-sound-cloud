#include "LEDCloudMatrix.hpp"

/**
 * @brief LEDMath functions can be found here on Desmos
 *  <a target="_blank" href="https://www.desmos.com/calculator/evodsxrtjj">Calculated Brightness Curves</a>
 */

/**
 * @brief Curve brightness input quadratically
 * 
 * @param x Input
 * @return uint8_t Transformed output
 */
uint8_t LEDCloudMatrix::LEDMath::quadraticBrightness(uint8_t x) {
	return (uint8_t) round(0.003921568627 * (double) pow((uint64_t) x, 2));
}

/**
 * @brief Curve brightness input cubically
 * 
 * @param x Input
 * @return uint8_t Transformed output
 */
uint8_t LEDCloudMatrix::LEDMath::cubicBrightness(uint8_t x) {
	return (uint8_t) round(0.0000153787005 * (double) pow((uint64_t) x, 3));
}

/**
 * @brief Curve brightness input quartically
 * 
 * @param x Input
 * @return uint8_t Transformed output
 */
uint8_t LEDCloudMatrix::LEDMath::quarticBrightness(uint8_t x) {
	return (uint8_t) round(0.0000000603086294 * (double) pow((uint64_t) x, 4));
}

/**
 * @brief Curve brightness input by inverse 4.10612163x^{0.745}
 * 
 * @param x Input
 * @return uint8_t Transformed output
 */
uint8_t LEDCloudMatrix::LEDMath::inverseFractional_0745_Brightness(uint8_t x) {
	return (uint8_t) round(4.1083529414 * (double) pow((uint64_t) x, 0.745));
}

/**
 * @brief Curve brightness input by inverse 12.444452145x^{0.545}
 * 
 * @param x Input
 * @return uint8_t Transformed output
 */
uint8_t LEDCloudMatrix::LEDMath::inverseFractional_0545_Brightness(uint8_t x) {
	return (uint8_t) round(12.444452148 * (double) pow((uint64_t) x, 0.545));
}

/**
 * @brief Produces Red component of 32-bit RGB
 * 
 * @param color 32-bit RGB color
 * @return uint8_t Red component (8-bit) of a 32-bit color
 */
uint8_t LEDCloudMatrix::LEDColor::redFromColor(uint32_t color) {
    return (color >> 16) & 0xFF;
}

/**
 * @brief Produces Green component of 32-bit RGB
 * 
 * @param color 32-bit RGB color
 * @return uint8_t Green component (8-bit) of a 32-bit color
 */
uint8_t LEDCloudMatrix::LEDColor::greenFromColor(uint32_t color) {
    return (color >> 8) & 0xFF;
}

/**
 * @brief Produces Blue component of 32-bit RGB
 * 
 * @param color 32-bit RGB color
 * @return uint8_t Blue component (8-bit) of a 32-bit color
 */
uint8_t LEDCloudMatrix::LEDColor::blueFromColor(uint32_t color) {
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
uint32_t LEDCloudMatrix::LEDColor::color(uint8_t r, uint8_t g, uint8_t b) {
	return ((uint32_t) r << 16) | ((uint32_t) g <<  8) | b;
}

/**
 * @brief Continuous color wheel function. Input a value 0 to 255 to get a color value. 
 * The colors are a transition r - g - b - back to r.
 * 
 * @param wheel_pos A number from 0 to 255 giving a position in the color wheel
 * @return uint32_t 
 */
uint32_t LEDCloudMatrix::LEDColor::colorWheel(uint8_t wheel_pos) {
	wheel_pos = 255 - wheel_pos;

	if (wheel_pos < 85) {
		return LEDCloudMatrix::LEDColor::color(255 - wheel_pos * 3, 0, wheel_pos * 3);
	}

	if (wheel_pos < 170) {
		wheel_pos -= 85;
		return LEDCloudMatrix::LEDColor::color(0, wheel_pos * 3, 255 - wheel_pos * 3);
	}

	wheel_pos -= 170;
	return LEDCloudMatrix::LEDColor::color(wheel_pos * 3, 255 - wheel_pos * 3, 0);
}

uint32_t LEDCloudMatrix::LEDColor::scaleColorSaturation(uint32_t color, uint8_t scale) {
	float s_f = scale / 255.0;
	uint8_t r = (uint8_t) round((float) redFromColor(color) * s_f);
	uint8_t g = (uint8_t) round((float) greenFromColor(color) * s_f);
	uint8_t b = (uint8_t) round((float) blueFromColor(color) * s_f);
	return LEDColor::color(r, g, b);
}
