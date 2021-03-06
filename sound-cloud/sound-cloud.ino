#include <inttypes.h>

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "audio/Audio.hpp"
#include "io/io.h"
#include "io/config.h"
#include "led/LEDCloudMatrix.hpp"
#include "led/LEDCloudControl.hpp"
#include "led/Animations.hpp"

// the arduino IDE struggles with C files nested in folders
#include "audio/Audio.cpp"
#include "io/io.c"
#include "led/LEDCloudMatrix.cpp"
#include "led/LEDCloudControl.cpp"
#include "led/Animations.cpp"
#include "led/Animations.Standby.cpp"
#include "led/Animations.Simple.cpp"
#include "led/Animations.Advanced.cpp"

using namespace LEDCloudMatrix;

Adafruit_NeoPixel row_1_2_led_strip(ROW_1_2_LED_LENGTH, ROW_1_2_LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel row_3_4_led_strip(ROW_3_4_LED_LENGTH, ROW_3_4_LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel top_led_strip(TOP_LED_LENGTH, TOP_LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
LEDCloudControl* led_ctrl;

uint8_t last_mode = 0;

void setup() {
	// initilize mode button interrupt
	io_init_mode_control();

	// initialize status LED
	io_init_stat_led();
	io_led_write_starting();

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
	led_segment_t* z_1__3 = led_ctrl->createSegment(strip_t::S_R34, 24, 11);
	led_segment_t* z_1__4 = led_ctrl->createSegment(strip_t::S_R34, 35, 1);
	led_ctrl->mapSegmentToZone(z_1__0, zone_t::FRONT, 0, 0, seg_o_t::ROW_POS);
	led_ctrl->mapSegmentToZone(z_1__1, zone_t::FRONT, 1, 12, seg_o_t::ROW_NEG);
	led_ctrl->mapSegmentToZone(z_1__2, zone_t::FRONT, 2, 0, seg_o_t::ROW_POS);
	led_ctrl->mapSegmentToZone(z_1__3, zone_t::FRONT, 3, 0, seg_o_t::ROW_POS);
	led_ctrl->mapSegmentToZone(z_1__4, zone_t::FRONT, 3, 12, seg_o_t::ROW_POS);

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

	// show that initialization worked
	io_led_write_active();
}

void loop() {
	uint8_t mode = io_get_mode();

	// execute animation based on current mode
	switch (mode) {
		case LED_MODE_BASS_SIDE_PULSE:
			Animations::Simple::bassSidePulse();
			break;
		case LED_MODE_BASS_RANGE_PULSE:
			Animations::Simple::bassRangePulse();
			break;
		case LED_MODE_BASS_TREBLE_PULSE:
			Animations::Simple::bassTreblePulse();
			break;
		case LED_MODE_LINEAR_VISUALIZER_RAINBOW:
			Animations::Advanced::rainbowLinearVisualizer3D();	
			break;
		case LED_MODE_LINEAR_VISUALIZER_FIRE:
			Animations::Advanced::fireLinearVisualizer3D();	
			break;
	}

	if (last_mode != mode) {
		// light status blue to indicate state change
		io_led_write_changing();

		// clear all LEDs and update last mode
		Animations::allOff();
		last_mode = mode;
		delay(100);

		// return to running status
		io_led_write_active();
	}
}
