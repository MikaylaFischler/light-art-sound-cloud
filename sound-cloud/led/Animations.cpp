#include "Animations.hpp"

LEDCloudMatrix* Animations::lcm = NULL;
Audio* Animations::audio = NULL;

/**
 * @brief Link a LEDCloudMatrix object for LED control
 * 
 * @param lcm Pointer to a LEDCloudMatrix object
 */
void Animations::linkCloudMatrix(LEDCloudMatrix* lcm) { Animations::lcm = lcm; }

/**
 * @brief Link an Audio object for audio input
 * 
 * @param audio Pointer to an Audio object
 */
void Animations::linkAudio(Audio* audio) { Animations::audio = audio; }

/**
 * @brief Turn off all LEDs
 * 
 */
void Animations::allOff(void) {
	// set strip 0 to all off
	uint16_t len = lcm->strip(lcm->ROW_1_2)->numPixels();
	for (uint16_t i = 0; i < len; i++) { lcm->strip(lcm->ROW_1_2)->setPixelColor(i, 0); }

	// set strip 1 to all off
	len = lcm->strip(lcm->ROW_3_4)->numPixels();
	for (uint16_t i = 0; i < len; i++) { lcm->strip(lcm->ROW_3_4)->setPixelColor(i, 0); }
	
	// set strip 2 to all off
	len = lcm->strip(lcm->TOP)->numPixels();
	for (uint16_t i = 0; i < len; i++) { lcm->strip(lcm->TOP)->setPixelColor(i, 0); }

	// show all off
	lcm->strip(lcm->ROW_1_2)->show();
	lcm->strip(lcm->ROW_3_4)->show();
	lcm->strip(lcm->TOP)->show();
}
