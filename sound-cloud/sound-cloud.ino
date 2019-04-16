
#ifdef Arduino_h
// Since the arduino IDE struggles with C files nested in folders, 
// we will include them only if arduino is being compiled here
#include "audio/Audio.cpp"
#include "io/io.c"
#include "led/LEDCloudMatrix.cpp"
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
}

void loop() {

}
