#include "Animations.hpp"

LEDCloudControl* Animations::lcm = NULL;

/**
 * @brief Link a LEDCloudControl object for LED control
 * 
 * @param lcm Pointer to a LEDCloudControl object
 */
void Animations::linkCloudControl(LEDCloudControl* lcm) { Animations::lcm = lcm; }

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

void Animations::diagnostics(void) {
	// test level 0
	for (int i = 0; i < lcm->levelLength(0); i++) {
		if (i > 0) { lcm->setPixelColorByLevel(0, i - 1, COLOR_OFF); }
		lcm->setPixelColorByLevel(0, i, COLOR_WHITE);
		lcm->showLevel(0);
		delay(30);
	}
	lcm->setPixelColorByLevel(0, lcm->levelLength(0) - 1, COLOR_OFF);
	lcm->showLevel(0);

	// test level 1
	for (int i = 0; i < lcm->levelLength(1); i++) {
		if (i > 0) { lcm->setPixelColorByLevel(1, i - 1, COLOR_OFF); }
		lcm->setPixelColorByLevel(1, i, COLOR_WHITE);
		lcm->showLevel(1);
		delay(30);
	}
	lcm->setPixelColorByLevel(1, lcm->levelLength(1) - 1, COLOR_OFF);
	lcm->showLevel(1);

	// test level 2
	for (int i = 0; i < lcm->levelLength(2); i++) {
		if (i > 0) { lcm->setPixelColorByLevel(2, i - 1, COLOR_OFF); }
		lcm->setPixelColorByLevel(2, i, COLOR_WHITE);
		lcm->showLevel(2);
		delay(30);
	}
	lcm->setPixelColorByLevel(2, lcm->levelLength(2) - 1, COLOR_OFF);
	lcm->showLevel(2);

	// test level 3
	for (int i = 0; i < lcm->levelLength(3); i++) {
		if (i > 0) { lcm->setPixelColorByLevel(3, i - 1, COLOR_OFF); }
		lcm->setPixelColorByLevel(3, i, COLOR_WHITE);
		lcm->showLevel(3);
		delay(30);
	}
	lcm->setPixelColorByLevel(3, lcm->levelLength(3) - 1, COLOR_OFF);
	lcm->showLevel(3);

	// test front and side rows
	lcm->setZoneRow(zone_t::RIGHT, 0, COLOR_GREEN);
	lcm->setZoneRow(zone_t::RIGHT, 1, COLOR_SEA_GREEN);
	lcm->setZoneRow(zone_t::RIGHT, 2, COLOR_BABY_BLUE);
	lcm->showZone(zone_t::RIGHT);

	lcm->setZoneRow(zone_t::FRONT, 0, COLOR_GREEN);
	lcm->setZoneRow(zone_t::FRONT, 1, COLOR_SEA_GREEN);
	lcm->setZoneRow(zone_t::FRONT, 2, COLOR_BABY_BLUE);
	lcm->setZoneRow(zone_t::FRONT, 3, COLOR_BLUE);
	lcm->showZone(zone_t::FRONT);

	lcm->setZoneRow(zone_t::LEFT, 0, COLOR_GREEN);
	lcm->setZoneRow(zone_t::LEFT, 1, COLOR_SEA_GREEN);
	lcm->setZoneRow(zone_t::LEFT, 2, COLOR_BABY_BLUE);
	lcm->setZoneRow(zone_t::LEFT, 3, COLOR_BLUE);
	lcm->showZone(zone_t::LEFT);

	delay(1000);

	// wipe right columns
	for (uint8_t i = 0; i < lcm->zoneWidth(zone_t::RIGHT); i++) {
		lcm->setZoneCol(zone_t::RIGHT, i, COLOR_RED);
		lcm->showZone(zone_t::RIGHT);
		delay(50);
		lcm->setZoneCol(zone_t::RIGHT, i, COLOR_OFF);
	}
	lcm->showZone(zone_t::RIGHT);

	// wipe front columns
	for (uint8_t i = 0; i < lcm->zoneWidth(zone_t::FRONT); i++) {
		lcm->setZoneCol(zone_t::FRONT, i, COLOR_RED);
		lcm->showZone(zone_t::FRONT);
		delay(50);
		lcm->setZoneCol(zone_t::FRONT, i, COLOR_OFF);
	}
	lcm->showZone(zone_t::FRONT);

	// wipe left columns
	for (uint8_t i = 0; i < lcm->zoneWidth(zone_t::LEFT); i++) {
		lcm->setZoneCol(zone_t::LEFT, i, COLOR_RED);
		lcm->showZone(zone_t::LEFT);
		delay(50);
		lcm->setZoneCol(zone_t::LEFT, i, COLOR_OFF);
	}
	lcm->showZone(zone_t::LEFT);

	// test top rows
	lcm->setZoneCol(zone_t::TOP, 13, COLOR_WHITE);
	lcm->setZoneRow(zone_t::TOP, 0, COLOR_RED);
	lcm->setZoneRow(zone_t::TOP, 1, COLOR_ORANGE);
	lcm->setZoneRow(zone_t::TOP, 2, COLOR_YELLOW);
	lcm->setZoneRow(zone_t::TOP, 3, COLOR_GREEN);
	lcm->setZoneRow(zone_t::TOP, 4, COLOR_BLUE);
	lcm->setZoneRow(zone_t::TOP, 5, COLOR_PURPLE);
	lcm->showZone(zone_t::TOP);

	delay(1000);

	// wipe top columns
	for (uint8_t i = 0; i < lcm->zoneWidth(zone_t::TOP); i++) {
		lcm->setZoneCol(zone_t::TOP, i, COLOR_WHITE);
		lcm->showZone(zone_t::TOP);
		delay(50);
		lcm->setZoneCol(zone_t::TOP, i, COLOR_OFF);
	}
	lcm->showZone(zone_t::TOP);
}
