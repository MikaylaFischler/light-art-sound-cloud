#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "audio/audio.cpp"
#include "io/io.c"
#include "io/config.h"
#include "led/LEDCloudMatrix.cpp"

Adafruit_NeoPixel led_strip(LED_LENGTH, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
LEDCloudMatrix* c_matrix;
struct audio audio;

void setup() {
	// initilize mode button interrupt
	io_init_mode_control();

	// initilize audio system
	audio_init(&audio);

	// init LEDs
	led_strip.begin();
	led_strip.show();

	// create our cloud matrix
	c_matrix = new LEDCloudMatrix(&led_strip);
}

void loop() {

}
