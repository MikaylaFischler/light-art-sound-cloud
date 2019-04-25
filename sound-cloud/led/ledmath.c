#ifndef CLOUD_LEDMATH_C_
#define CLOUD_LEDMATH_C_

/**
	@file ledmath.c
  	@brief This file contains mathematical utility functions for dorm LEDs.<br/>
	<a target="_blank" href="https://www.desmos.com/calculator/evodsxrtjj">Calculated Brightness Curves</a>
*/

/* ~~~ Brightness Curves ~~~ */
/* Curve input to bias towards lower outputs to cause more noticeable changes in brightness */

/**
 * @brief Curve brightness input quadratically
 * 
 * @param x Input
 * @return uint8_t Transformed output
 */
uint8_t quadraticBrightness(uint8_t x) {
	return (uint8_t) round(0.003921568627 * (double) pow((uint64_t) x, 2));
}

/**
 * @brief Curve brightness input cubically
 * 
 * @param x Input
 * @return uint8_t Transformed output
 */
uint8_t cubicBrightness(uint8_t x) {
	return (uint8_t) round(0.0000153787005 * (double) pow((uint64_t) x, 3));
}

/**
 * @brief Curve brightness input quartically
 * 
 * @param x Input
 * @return uint8_t Transformed output
 */
uint8_t quarticBrightness(uint8_t x) {
	return (uint8_t) round(0.0000000603086294 * (double) pow((uint64_t) x, 4));
}

/**
 * @brief Curve brightness input by inverse 4.10612163x^{0.745}
 * 
 * @param x Input
 * @return uint8_t Transformed output
 */
uint8_t inverseFractional_0745_Brightness(uint8_t x) {
	return (uint8_t) round(4.1083529414 * (double) pow((uint64_t) x, 0.745));
}

/**
 * @brief Curve brightness input by inverse 12.444452145x^{0.545}
 * 
 * @param x Input
 * @return uint8_t Transformed output
 */
uint8_t inverseFractional_0545_Brightness(uint8_t x) {
	return (uint8_t) round(12.444452148 * (double) pow((uint64_t) x, 0.545));
}

#endif
