#include "Animations.hpp"

void Animations::Standby::solidSoftWhite(void) {
	for (uint8_t z = zone_t::RIGHT; z <= zone_t::TOP; z++) {
		for (uint16_t y = 0; y < lcm->zoneHeight((zone_t) z); y++) {
			for (uint16_t x = 0; x < lcm->zoneWidth((zone_t) z); x++) {
				lcm->setPixelColorByZone((zone_t) z, x, y, COLOR_DIM_OFF_WHITE);
				Serial.print(x);
				Serial.print(", ");
				Serial.println(y);
			}
		}
	}

	lcm->show();
}

void Animations::Standby::whiteWave(void) {
	// for (uint8_t i = 0; i < lcm->zoneWidth(zone_t::TOP); i++) {
		
	// }	
}

void Animations::Standby::cloudToCloudLighting(void) {

}

void Animations::Standby::rainbowWash(void) {

}

void Animations::Standby::rainbowFade(void) {

}
