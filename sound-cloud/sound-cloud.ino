#include <inttypes.h>

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "audio/Audio.hpp"
#include "io/io.h"
#include "io/config.h"
#include "led/LEDCloudMatrix.hpp"
#include "led/Animations.hpp"

// the arduino IDE struggles with C files nested in folders
#include "audio/Audio.cpp"
#include "io/io.c"
#include "led/LEDCloudMatrix.cpp"
#include "led/Animations.cpp"
#include "led/Animations.Standby.cpp"

using namespace LEDCloudMatrix;

Adafruit_NeoPixel row_1_2_led_strip(ROW_1_2_LED_LENGTH, ROW_1_2_LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel row_3_4_led_strip(ROW_3_4_LED_LENGTH, ROW_3_4_LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel top_led_strip(TOP_LED_LENGTH, TOP_LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
LEDCloudControl* led_ctrl;

void setup() {
	// initilize mode button interrupt
	io_init_mode_control();

	// initilize audio system
	Audio::init();

	// init LEDs
	row_1_2_led_strip.begin();
	row_3_4_led_strip.begin();
	top_led_strip.begin();

	// create our cloud matrix
	led_ctrl = new LEDCloudControl(&row_1_2_led_strip, &row_3_4_led_strip, &top_led_strip);

	// set up matrix segmentation

	// levels
	led_segment_t* level_0 = led_ctrl->createSegment(strip_t::S_R12, 0, 23);
	led_segment_t* level_1 = led_ctrl->createSegment(strip_t::S_R12, 23, 23);
	led_segment_t* level_2 = led_ctrl->createSegment(strip_t::S_R34, 0, 21);
	led_segment_t* level_3 = led_ctrl->createSegment(strip_t::S_R34, 21, 19);
	led_ctrl->mapSegmentToLevel(level_0, 0);
	led_ctrl->mapSegmentToLevel(level_1, 1);
	led_ctrl->mapSegmentToLevel(level_2, 2);
	led_ctrl->mapSegmentToLevel(level_3, 3);

	// right zone
	led_segment_t* z_0__0 = led_ctrl->createSegment(strip_t::S_R12, 0, 5);
	led_segment_t* z_0__1 = led_ctrl->createSegment(strip_t::S_R12, 41, 5);
	led_segment_t* z_0__2 = led_ctrl->createSegment(strip_t::S_R34, 0, 1);
	led_ctrl->mapSegmentToZone(z_0__0, zone_t::RIGHT, 0, 0, seg_o_t::ROW_POS);
	led_ctrl->mapSegmentToZone(z_0__1, zone_t::RIGHT, 1, 4, seg_o_t::ROW_NEG);
	led_ctrl->mapSegmentToZone(z_0__2, zone_t::RIGHT, 2, 4, seg_o_t::ROW_POS);

	// front zone
	led_segment_t* z_1__0 = led_ctrl->createSegment(strip_t::S_R12, 5, 12);
	led_segment_t* z_1__1 = led_ctrl->createSegment(strip_t::S_R12, 28, 13);
	led_segment_t* z_1__2 = led_ctrl->createSegment(strip_t::S_R34, 1, 13);
	led_segment_t* z_1__3 = led_ctrl->createSegment(strip_t::S_R34, 24, 12);
	led_ctrl->mapSegmentToZone(z_1__0, zone_t::FRONT, 0, 0, seg_o_t::ROW_POS);
	led_ctrl->mapSegmentToZone(z_1__1, zone_t::FRONT, 1, 12, seg_o_t::ROW_NEG);
	led_ctrl->mapSegmentToZone(z_1__2, zone_t::FRONT, 2, 0, seg_o_t::ROW_POS);
	led_ctrl->mapSegmentToZone(z_1__3, zone_t::FRONT, 3, 0, seg_o_t::ROW_POS);

	// left zone
	led_segment_t* z_2__0 = led_ctrl->createSegment(strip_t::S_R12, 17, 6);
	led_segment_t* z_2__1 = led_ctrl->createSegment(strip_t::S_R12, 23, 5);
	led_segment_t* z_2__2 = led_ctrl->createSegment(strip_t::S_R34, 14, 7);
	led_ctrl->mapSegmentToZone(z_2__0, zone_t::LEFT, 0, 0, seg_o_t::ROW_POS);
	led_ctrl->mapSegmentToZone(z_2__1, zone_t::LEFT, 1, 4, seg_o_t::ROW_NEG);
	led_ctrl->mapSegmentToZone(z_2__2, zone_t::LEFT, 2, 0, seg_o_t::ROW_POS);

	// top zone
	led_segment_t* z_3__0 = led_ctrl->createSegment(strip_t::S_TOP, 0, 11);
	led_segment_t* z_3__1 = led_ctrl->createSegment(strip_t::S_TOP, 11, 12);
	led_segment_t* z_3__2 = led_ctrl->createSegment(strip_t::S_TOP, 23, 12);
	led_segment_t* z_3__3 = led_ctrl->createSegment(strip_t::S_TOP, 35, 13);
	led_segment_t* z_3__4 = led_ctrl->createSegment(strip_t::S_TOP, 48, 12);
	led_segment_t* z_3__5 = led_ctrl->createSegment(strip_t::S_R34, 21, 3);
	led_segment_t* z_3__6 = led_ctrl->createSegment(strip_t::S_R34, 36, 6);
	led_segment_t* z_3__7 = led_ctrl->createSegment(strip_t::S_R34, 42, 1);
	led_ctrl->mapSegmentToZone(z_3__0, zone_t::TOP, 0, 2, seg_o_t::ROW_POS);
	led_ctrl->mapSegmentToZone(z_3__1, zone_t::TOP, 1, 12, seg_o_t::ROW_NEG);
	led_ctrl->mapSegmentToZone(z_3__2, zone_t::TOP, 2, 1, seg_o_t::ROW_POS);
	led_ctrl->mapSegmentToZone(z_3__3, zone_t::TOP, 3, 12, seg_o_t::ROW_NEG);
	led_ctrl->mapSegmentToZone(z_3__4, zone_t::TOP, 4, 0, seg_o_t::ROW_POS);
	led_ctrl->mapSegmentToZone(z_3__5, zone_t::TOP, 2, 0, seg_o_t::COLUMN_NEG);
	led_ctrl->mapSegmentToZone(z_3__6, zone_t::TOP, 0, 13, seg_o_t::COLUMN_POS);
	led_ctrl->mapSegmentToZone(z_3__7, zone_t::TOP, 4, 12, seg_o_t::COLUMN_POS);

	// setup animations system
	Animations::linkCloudControl(led_ctrl);

	// ensure all are shown as off
	led_ctrl->show();
}

void loop() {
	// Serial.println(led_ctrl->levelLength(0));

	// LEVEL DEBUG START
	for (int i = 0; i < led_ctrl->levelLength(0); i++) {
		if (i > 0) { led_ctrl->setPixelColorByLevel(0, i - 1, COLOR_OFF); }
		led_ctrl->setPixelColorByLevel(0, i, COLOR_WHITE);
		led_ctrl->showLevel(0);
		delay(30);
	}
	led_ctrl->setPixelColorByLevel(0, led_ctrl->levelLength(0) - 1, COLOR_OFF);
	led_ctrl->showLevel(0);

	for (int i = 0; i < led_ctrl->levelLength(1); i++) {
		if (i > 0) { led_ctrl->setPixelColorByLevel(1, i - 1, COLOR_OFF); }
		led_ctrl->setPixelColorByLevel(1, i, COLOR_WHITE);
		led_ctrl->showLevel(1);
		delay(30);
	}
	led_ctrl->setPixelColorByLevel(1, led_ctrl->levelLength(1) - 1, COLOR_OFF);
	led_ctrl->showLevel(1);

	for (int i = 0; i < led_ctrl->levelLength(2); i++) {
		if (i > 0) { led_ctrl->setPixelColorByLevel(2, i - 1, COLOR_OFF); }
		led_ctrl->setPixelColorByLevel(2, i, COLOR_WHITE);
		led_ctrl->showLevel(2);
		delay(30);
	}
	led_ctrl->setPixelColorByLevel(2, led_ctrl->levelLength(2) - 1, COLOR_OFF);
	led_ctrl->showLevel(2);

	for (int i = 0; i < led_ctrl->levelLength(3); i++) {
		if (i > 0) { led_ctrl->setPixelColorByLevel(3, i - 1, COLOR_OFF); }
		led_ctrl->setPixelColorByLevel(3, i, COLOR_WHITE);
		led_ctrl->showLevel(3);
		delay(30);
	}
	led_ctrl->setPixelColorByLevel(3, led_ctrl->levelLength(3) - 1, COLOR_OFF);
	led_ctrl->showLevel(3);
	// LEVEL DEBUG END

	// led_ctrl->setPixelColorByZone(zone_t::TOP, 3, 0, COLOR_RED);
	// led_ctrl->setPixelColorByZone(zone_t::TOP, 2, 0, COLOR_RED);
	// led_ctrl->setPixelColorByZone(zone_t::TOP, 1, 0, COLOR_RED);
	// led_ctrl->setPixelColorByZone(zone_t::TOP, 0, 0, COLOR_RED);
	// led_ctrl->showZone(zone_t::TOP);

	led_ctrl->setZoneRow(zone_t::RIGHT, 0, COLOR_GREEN);
	led_ctrl->setZoneRow(zone_t::RIGHT, 1, COLOR_SEA_GREEN);
	led_ctrl->setZoneRow(zone_t::RIGHT, 2, COLOR_BABY_BLUE);
	led_ctrl->showZone(zone_t::RIGHT);

	led_ctrl->setZoneRow(zone_t::FRONT, 0, COLOR_GREEN);
	led_ctrl->setZoneRow(zone_t::FRONT, 1, COLOR_SEA_GREEN);
	led_ctrl->setZoneRow(zone_t::FRONT, 2, COLOR_BABY_BLUE);
	led_ctrl->setZoneRow(zone_t::FRONT, 3, COLOR_BLUE);
	led_ctrl->showZone(zone_t::FRONT);

	led_ctrl->setZoneRow(zone_t::LEFT, 0, COLOR_GREEN);
	led_ctrl->setZoneRow(zone_t::LEFT, 1, COLOR_SEA_GREEN);
	led_ctrl->setZoneRow(zone_t::LEFT, 2, COLOR_BABY_BLUE);
	led_ctrl->setZoneRow(zone_t::LEFT, 3, COLOR_BLUE);
	led_ctrl->showZone(zone_t::LEFT);

	delay(1000);

	for (uint8_t i = 0; i < led_ctrl->zoneWidth(zone_t::RIGHT); i++) {
		led_ctrl->setZoneCol(zone_t::RIGHT, i, COLOR_RED);
		led_ctrl->showZone(zone_t::RIGHT);
		delay(50);
		led_ctrl->setZoneCol(zone_t::RIGHT, i, COLOR_OFF);
	}
	led_ctrl->showZone(zone_t::RIGHT);

	for (uint8_t i = 0; i < led_ctrl->zoneWidth(zone_t::FRONT); i++) {
		led_ctrl->setZoneCol(zone_t::FRONT, i, COLOR_RED);
		led_ctrl->showZone(zone_t::FRONT);
		delay(50);
		led_ctrl->setZoneCol(zone_t::FRONT, i, COLOR_OFF);
	}
	led_ctrl->showZone(zone_t::FRONT);

	for (uint8_t i = 0; i < led_ctrl->zoneWidth(zone_t::LEFT); i++) {
		led_ctrl->setZoneCol(zone_t::LEFT, i, COLOR_RED);
		led_ctrl->showZone(zone_t::LEFT);
		delay(50);
		led_ctrl->setZoneCol(zone_t::LEFT, i, COLOR_OFF);
	}
	led_ctrl->showZone(zone_t::LEFT);

	led_ctrl->setZoneCol(zone_t::TOP, 13, COLOR_WHITE);
	led_ctrl->setZoneRow(zone_t::TOP, 0, COLOR_RED);
	led_ctrl->setZoneRow(zone_t::TOP, 1, COLOR_ORANGE);
	led_ctrl->setZoneRow(zone_t::TOP, 2, COLOR_YELLOW);
	led_ctrl->setZoneRow(zone_t::TOP, 3, COLOR_GREEN);
	led_ctrl->setZoneRow(zone_t::TOP, 4, COLOR_BLUE);
	led_ctrl->setZoneRow(zone_t::TOP, 5, COLOR_PURPLE);
	led_ctrl->showZone(zone_t::TOP);

	delay(1000);

	for (uint8_t i = 0; i < led_ctrl->zoneWidth(zone_t::TOP); i++) {
		led_ctrl->setZoneCol(zone_t::TOP, i, COLOR_WHITE);
		led_ctrl->showZone(zone_t::TOP);
		delay(50);
		led_ctrl->setZoneCol(zone_t::TOP, i, COLOR_OFF);
	}
	led_ctrl->showZone(zone_t::TOP);

	// Animations::Standby::solidSoftWhite();
	// Serial.println("HEARTBEAT");
}
