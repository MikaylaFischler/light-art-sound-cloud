#include "Animations.hpp"

uint8_t Animations::Advanced::output_stream[9][14];

/**
 * @brief Draw a moving vizualizer of audio in blue
 * 
 */
void Animations::Advanced::solidLinearVisualizer3D(void) {
	unsigned long t_1 = micros();
	Audio::getFFTWhenReadyBlocking();

	// shift data up for historical view
	__shift_output_stream();

	for (uint8_t i = 0; i < 14; i++) {
		float avg = Audio::averageFFTRange(AUDIO_FFT_COMBINED, FREQ_43HZ + (3 * i), FREQ_129HZ + (3 * i));
		uint8_t out = Audio::fftToInt(avg, 6, cubicBrightness);
		output_stream[0][i] = out;
	}

	for (uint8_t row = 0; row < 9; row++) {
		zone_t z = (row > 3) ? zone_t::TOP : zone_t::FRONT;
		uint8_t y = (row > 3) ? row - 4 : row;

		for (uint8_t i = 0; i < 14; i++) {
			lcm->setPixelColorByZone(z, i, y, 0, 0, output_stream[row][i]);
		}
	}

	lcm->show();

	unsigned long t_2 = micros();
	delay(20);

	Serial.println(t_2 - t_1);
}

/**
 * @brief Draw a moving vizualizer of audio in a rainbow
 * 
 */
void Animations::Advanced::rainbowLinearVisualizer3D(void) {
	static uint32_t idx_to_color[14] = { COLOR_MAROON, COLOR_RED, COLOR_ORANGE, 
		COLOR_YELLOW, COLOR_GREEN, COLOR_SEA_GREEN, COLOR_LIGHT_BLUE, COLOR_BLUE, 
		COLOR_INDIGO, COLOR_PURPLE, COLOR_MAGENTA, COLOR_PINK, COLOR_DIM_OFF_WHITE, 
		COLOR_WHITE };

	unsigned long t_1 = micros();
	Audio::getFFTWhenReadyBlocking();

	// shift data up for historical view
	__shift_output_stream();

	for (uint8_t i = 0; i < 14; i++) {
		float avg = Audio::averageFFTRange(AUDIO_FFT_COMBINED, FREQ_43HZ + (3 * i), FREQ_129HZ + (3 * i));
		uint8_t out = Audio::fftToInt(avg, 6, cubicBrightness);
		output_stream[0][i] = out;
	}

	for (uint8_t row = 0; row < 9; row++) {
		zone_t z = (row > 3) ? zone_t::TOP : zone_t::FRONT;
		uint8_t y = (row > 3) ? row - 4 : row;

		for (uint8_t i = 0; i < 14; i++) {
			
			lcm->setPixelColorByZone(z, i, y, scaleColorSaturation(idx_to_color[i], output_stream[row][i]));
		}
	}

	lcm->show();

	unsigned long t_2 = micros();
	delay(20);

	Serial.println(t_2 - t_1);
}

/**
 * @brief Draw a moving vizualizer of audio in a fire style
 * 
 */
void Animations::Advanced::fireLinearVisualizer3D(void) {
	uint32_t idx_to_color[9] = {COLOR_WHITE, COLOR_RED, COLOR_RED, COLOR_ORANGE, COLOR_ORANGE, COLOR_YELLOW, COLOR_YELLOW,
									COLOR_DIM_OFF_WHITE, COLOR_DIM_OFF_WHITE };

	unsigned long t_1 = micros();
	Audio::getFFTWhenReadyBlocking();

	// shift data up for historical view
	__shift_output_stream();

	for (uint8_t i = 0; i < 14; i++) {
		float avg = Audio::averageFFTRange(AUDIO_FFT_COMBINED, FREQ_43HZ + (3 * i), FREQ_129HZ + (3 * i));
		uint8_t out = Audio::fftToInt(avg, 6, cubicBrightness);
		output_stream[0][i] = out;
	}

	for (uint8_t row = 0; row < 9; row++) {
		zone_t z = (row > 3) ? zone_t::TOP : zone_t::FRONT;
		uint8_t y = (row > 3) ? row - 4 : row;

		for (uint8_t i = 0; i < 14; i++) {
			
			lcm->setPixelColorByZone(z, i, y, scaleColorSaturation(idx_to_color[row], output_stream[row][i]));
		}
	}

	lcm->show();

	unsigned long t_2 = micros();
	delay(20);
	
	Serial.println(t_2 - t_1);
}

/**
 * @brief Shift the output stream upwards, making space for new data
 * 
 */
void Animations::Advanced::__shift_output_stream(void) {
	for (uint8_t i = 8; i > 0; i--) {
		for (uint8_t v = 0; v < 14; v++) {
			output_stream[i][v] = output_stream[i - 1][v];
		}
	}
}
