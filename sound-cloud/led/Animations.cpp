#include "Animations.hpp"

LEDCloudControl* Animations::lcm = NULL;
Audio* Animations::audio = NULL;

/**
 * @brief Link a LEDCloudControl object for LED control
 * 
 * @param lcm Pointer to a LEDCloudControl object
 */
void Animations::linkCloudControl(LEDCloudControl* lcm) { Animations::lcm = lcm; }

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
	uint16_t len = lcm->strip(strip_t::S_R12)->numPixels();
	for (uint16_t i = 0; i < len; i++) { lcm->strip(strip_t::S_R12)->setPixelColor(i, 0); }

	// set strip 1 to all off
	len = lcm->strip(strip_t::S_R34)->numPixels();
	for (uint16_t i = 0; i < len; i++) { lcm->strip(strip_t::S_R34)->setPixelColor(i, 0); }
	
	// set strip 2 to all off
	len = lcm->strip(strip_t::S_TOP)->numPixels();
	for (uint16_t i = 0; i < len; i++) { lcm->strip(strip_t::S_TOP)->setPixelColor(i, 0); }

	// show all off
	lcm->strip(strip_t::S_R12)->show();
	lcm->strip(strip_t::S_R34)->show();
	lcm->strip(strip_t::S_TOP)->show();
}
