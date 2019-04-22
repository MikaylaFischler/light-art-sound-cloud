
#ifdef Arduino_h
// Since the arduino IDE struggles with C files nested in folders, 
// we will include them only if arduino is being compiled here
#include "audio/Audio.cpp"

#include "io/io.c"

#include "led/LEDCloudMatrix.cpp"

#include "led/Animations.cpp"
#include "led/Animations.Standby.cpp"
#endif

#include <Adafruit_NeoPixel.h>

#include "audio/Audio.hpp"
#include "io/io.h"
#include "io/config.h"
#include "led/LEDCloudMatrix.hpp"
#include "led/Animations.hpp"

Adafruit_NeoPixel row_1_2_led_strip(ROW_1_2_LED_LENGTH, ROW_1_2_LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel row_3_4_led_strip(ROW_3_4_LED_LENGTH, ROW_3_4_LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel top_led_strip(TOP_LED_LENGTH, TOP_LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
LEDCloudMatrix* c_matrix;
Audio* audio;

void setup() {
	// initilize mode button interrupt
	io_init_mode_control();

	// initilize audio system
	audio = new Audio();

	// init LEDs
	row_1_2_led_strip.begin();
	row_1_2_led_strip.show();
	row_3_4_led_strip.begin();
	row_3_4_led_strip.show();
	top_led_strip.begin();
	top_led_strip.show();

	// create our cloud matrix
	c_matrix = new LEDCloudMatrix(&row_1_2_led_strip, &row_3_4_led_strip, &top_led_strip);

	// set up matrix segmentation
	led_segment_t* level_0 = c_matrix->createSegment(LEDCloudMatrix::ROW_1_2, 0, 23);
	led_segment_t* level_1 = c_matrix->createSegment(LEDCloudMatrix::ROW_1_2, 23, 23);
	led_segment_t* level_2 = c_matrix->createSegment(LEDCloudMatrix::ROW_3_4, 0, 21);
	led_segment_t* level_3 = c_matrix->createSegment(LEDCloudMatrix::ROW_3_4, 21, 19);

	led_segment_t* top_0  = c_matrix->createSegment(LEDCloudMatrix::TOP, 0, 11);
	led_segment_t* top_0_ = c_matrix->createSegment(LEDCloudMatrix::ROW_3_4, 40, 1);
	led_segment_t* top_1  = c_matrix->createSegment(LEDCloudMatrix::TOP, 11, 12);
	led_segment_t* top_1_ = c_matrix->createSegment(LEDCloudMatrix::ROW_3_4, 41, 1);
	led_segment_t* top_2  = c_matrix->createSegment(LEDCloudMatrix::TOP, 23, 12);
	led_segment_t* top_2_ = c_matrix->createSegment(LEDCloudMatrix::ROW_3_4, 42, 1);
	led_segment_t* top_3  = c_matrix->createSegment(LEDCloudMatrix::TOP, 35, 13);
	led_segment_t* top_3_ = c_matrix->createSegment(LEDCloudMatrix::TOP, 48, 12);

	Serial.println("MEMES");
	c_matrix->mapSegmentToLevel(level_0, 0);
	Serial.println("MEMES");
	c_matrix->mapSegmentToLevel(level_1, 1);
	Serial.println("MEMES");
	c_matrix->mapSegmentToLevel(level_2, 2);
	Serial.println("MEMES");
	c_matrix->mapSegmentToLevel(level_3, 3);
	Serial.println("MEMES");

	// c_matrix->mapSegmentToTopMatrix()


	// setup animations system
	Animations::linkAudio(audio);
	Animations::linkCloudMatrix(c_matrix);
}

void loop() {
	for (int i = 0; i < c_matrix->levelLength(0); i++) {
		c_matrix->setPixelColorByLevel(0, i, COLOR_DIM_OFF_WHITE);
		Serial.println(i);
	}
	c_matrix->showLevel(0);
	delay(1000);
	// Animations::Standby::solidSoftWhite();
	// Serial.println("HEARTBEAT");
}
