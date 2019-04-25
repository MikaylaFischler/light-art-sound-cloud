#include "Audio.hpp"

AudioControlSGTL5000* Audio::board = NULL;
AudioInputI2S* Audio::input = NULL;
AudioAnalyzeFFT1024* Audio::fft_l = NULL;
AudioAnalyzeFFT1024* Audio::fft_r = NULL;
AudioConnection* Audio::fft_l_conn = NULL;
AudioConnection* Audio::fft_r_conn = NULL;
float** Audio::last_fft = NULL;

uint8_t Audio::ready = 0;

/**
 * @brief Initialize the audio system
 * 
 */
void Audio::init(void) {
	if (!ready) {
		// allocate audio memory
		AudioMemory(20);

		// create objects for audio processing
		board = new AudioControlSGTL5000();
		input = new AudioInputI2S();
		fft_l = new AudioAnalyzeFFT1024();
		fft_r = new AudioAnalyzeFFT1024();
		fft_l_conn = new AudioConnection(*input, 0, *fft_l, 0);
		fft_r_conn = new AudioConnection(*input, 0, *fft_r, 0);

		// last FFT data set
		last_fft = (float**) malloc(sizeof(float*) * 2);
		last_fft[AUDIO_FFT_LEFT] = (float*) malloc(sizeof(float) * 512);
		last_fft[AUDIO_FFT_RIGHT] = (float*) malloc(sizeof(float) * 512);

		// setup board
		board->enable();
		board->inputSelect(AUDIO_INPUT_LINEIN);
		board->volume(0.5);
		board->lineInLevel(15);

		ready = 1;
	}
}

/**
 * @brief Get the FFT data if available
 * 
 * @return Pointer to FFT array of last collected data
 */
float** Audio::getFFT(void) {
	// check availability
	if (!(fft_l->available() && fft_r->available())) {
		return last_fft;
	}

	// read data
	for (uint16_t i = 0; i < 512; i++) {
		last_fft[AUDIO_FFT_LEFT][i] = fft_l->read(i);
		last_fft[AUDIO_FFT_RIGHT][i] = fft_r->read(i);
	}

	// return the pointer for ease of use
	return last_fft;
}

/**
 * @brief Get the FFT data if available, if not, returns NULL
 * 
 * @return Pointer to FFT array of the collected data or NULL if not ready
 */
float** Audio::getFFTWhenReady(void) {
	// return if unavailable
	if (!(fft_l->available() && fft_r->available())) { return NULL; }

	// read data
	for (uint16_t i = 0; i < 512; i++) {
		last_fft[AUDIO_FFT_LEFT][i] = fft_l->read(i);
		last_fft[AUDIO_FFT_RIGHT][i] = fft_r->read(i);
	}

	// return the pointer for ease of use
	return last_fft;
}

/**
 * @brief Get the FFT data if available, if not, this becomes blocking until data is available
 * 
 * @return Pointer to FFT array of the collected data
 */
float** Audio::getFFTWhenReadyBlocking(void) {
	// wait for availability
	while (!(fft_l->available() && fft_r->available())) { __asm__ __volatile__ ("nop\n\t"); }

	// read data
	for (uint16_t i = 0; i < 512; i++) {
		last_fft[AUDIO_FFT_LEFT][i] = fft_l->read(i);
		last_fft[AUDIO_FFT_RIGHT][i] = fft_r->read(i);
	}

	// return the pointer for ease of use
	return last_fft;
}

/**
 * @brief Average together a range of FFT data points
 * 
 * @param side Either AUDIO_FFT_LEFT, AUDIO_FFT_RIGHT, or AUDIO_FFT_COMBINED to combine them (for sepearte data, use one of the averageDualFFTRange functions)
 * @param bin_start The start index (inclusive, min 0, max 511)
 * @param bin_end The end index (inclusive, min 0, max 511)
 * @return float The average of the FFT range
 */
float Audio::averageFFTRange(uint8_t side, uint16_t bin_start, uint16_t bin_end) {
	if (bin_start > 511) { bin_start == 511; }
	if (bin_end > 511) { bin_end == 511; }

	float avg = 0.0;

	if (side == AUDIO_FFT_LEFT || side == AUDIO_FFT_RIGHT) {
		for (uint8_t i = bin_start; i <= bin_end; i++) {
			avg += Audio::last_fft[side][i];
		}

		avg /= bin_end - bin_start + 1;
	} else if (side == AUDIO_FFT_COMBINED) {
		for (uint8_t i = bin_start; i <= bin_end; i++) {
			avg += Audio::last_fft[AUDIO_FFT_LEFT][i];
			avg += Audio::last_fft[AUDIO_FFT_RIGHT][i];
		}

		avg /= (bin_end - bin_start + 1) * 2.0;
	}

	return avg;
}

/**
 * @brief 'Average' together a range of FFT data points. This sums the data, but divides not by the count, but by the provided division_factor
 * 
 * @param side Either AUDIO_FFT_LEFT, AUDIO_FFT_RIGHT, or AUDIO_FFT_COMBINED to combine them (for sepearte data, use one of the averageDualFFTRange functions) 
 * @param bin_start The start index (inclusive, min 0, max 511)
 * @param bin_end The end index (inclusive, min 0, max 511)
 * @param division_factor The factor to divide the sum by (instead of dividing by the number of points summed)
 * @return float The scaled 'pseudo-average' of the FFT range
 */
float Audio::averageFFTRangeUnbalanced(uint8_t side, uint16_t bin_start, uint16_t bin_end, float division_factor) {
	if (bin_start > 511) { bin_start == 511; }
	if (bin_end > 511) { bin_end == 511; }

	float avg = 0.0;

	if (side == AUDIO_FFT_LEFT || side == AUDIO_FFT_RIGHT) {
		for (uint8_t i = bin_start; i <= bin_end; i++) {
			avg += Audio::last_fft[side][i];
		}

		avg /= division_factor;
	} else if (side == AUDIO_FFT_COMBINED) {
		for (uint8_t i = bin_start; i <= bin_end; i++) {
			avg += Audio::last_fft[AUDIO_FFT_LEFT][i];
			avg += Audio::last_fft[AUDIO_FFT_RIGHT][i];
		}

		avg /= (division_factor) * 2.0;
	}

	return avg;
}

/**
 * @brief 'Average' together a range of FFT data points. This sums the data, but divides not by the count, but by the provided division_factor. 
 * Returns a 2 element array, containing the left then the right data.
 * 
 * @param bin_start The start index (inclusive, min 0, max 511)
 * @param bin_end The end index (inclusive, min 0, max 511)
 * @param division_factor The factor to divide the sum by (instead of dividing by the number of points summed)
 * @return float The scaled 'pseudo-average' of the FFT range [left, right]
 */
float* Audio::averageDualFFTRange(uint16_t bin_start, uint16_t bin_end) {
	if (bin_start > 511) { bin_start == 511; }
	if (bin_end > 511) { bin_end == 511; }

	static float avgs[2] = { 0.0, 0.0 };

	for (uint8_t i = bin_start; i <= bin_end; i++) {
		avgs[0] += Audio::last_fft[AUDIO_FFT_LEFT][i];
		avgs[1] += Audio::last_fft[AUDIO_FFT_RIGHT][i];
	}

	avgs[0] /= bin_end - bin_start + 1;
	avgs[1] /= bin_end - bin_start + 1;

	return avgs;
}

/**
 * @brief 'Average' together a range of FFT data points. This sums the data, but divides not by the count, but by the provided division_factor. 
 * Returns a 2 element array, containing the left then the right data.
 * 
 * @param bin_start The start index (inclusive, min 0, max 511)
 * @param bin_end The end index (inclusive, min 0, max 511)
 * @param division_factor The factor to divide the sum by (instead of dividing by the number of points summed)
 * @return float* The scaled 'pseudo-averages' of the FFT range [left, right]
 */
float* Audio::averageDualFFTRangeUnbalanced(uint16_t bin_start, uint16_t bin_end, float division_factor) {
	if (bin_start > 511) { bin_start == 511; }
	if (bin_end > 511) { bin_end == 511; }

	static float avgs[2] = { 0.0, 0.0 };

	for (uint8_t i = bin_start; i <= bin_end; i++) {
		avgs[0] += Audio::last_fft[AUDIO_FFT_LEFT][i];
		avgs[1] += Audio::last_fft[AUDIO_FFT_RIGHT][i];
	}

	avgs[0] /= division_factor;
	avgs[1] /= division_factor;

	return avgs;
}

/**
 * @brief Convert an FFT float bin to a integer
 * 
 * @param bin The float value
 * @param scale_factor Scale the input float by this before conversion
 * @param brightness_transform This allows transformation of the output before returning
 * @return uint8_t 0 to 255 for LED control
 */
uint8_t Audio::fftToInt(float bin, float scale_factor, uint8_t (*brightness_transform)(uint64_t)) { return (uint8_t) round(bin * 255); }
