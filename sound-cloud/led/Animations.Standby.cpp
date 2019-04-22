#include "Animations.hpp"

void Animations::Standby::solidSoftWhite(void) {
	for (uint16_t y = 0; y < lcm->fullMatrixHeight(); y++) {
		for (uint16_t x = 0; x < lcm->fullMatrixWidth(); x++) {
			lcm->setPixelColorByFullMatrix(x, y, COLOR_DIM_OFF_WHITE);
			Serial.print(x);
			Serial.print(", ");
			Serial.println(y);
		}
	}

	lcm->show();
}

void Animations::Standby::whiteWave(void) {
	for (uint8_t i = 0; i < lcm->fullMatrixWidth(); i++) {
		
	}	
}

void Animations::Standby::cloudToCloudLighting(void) {

}

void Animations::Standby::rainbowWash(void) {

}

void Animations::Standby::rainbowFade(void) {

}
