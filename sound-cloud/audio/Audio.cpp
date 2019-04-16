#include "Audio.hpp"

/**
 * @brief Initialize an audio object
 * 
 * @param audio Pointer to audio object
 */
Audio::Audio(void) {
	// create objects for audio processing
	this->board = new AudioInputI2S();
	this->fft_l = new AudioAnalyzeFFT1024();
	this->fft_r = new AudioAnalyzeFFT1024();
	this->fft_l_conn = new AudioConnection(*this->board, 1, *this->fft_l, 0);
	this->fft_r_conn = new AudioConnection(*this->board, 0, *this->fft_r, 0);

	// last FFT data set
	this->last_fft = (float**) malloc(sizeof(float*) * 2);
	this->last_fft[AUDIO_FFT_LEFT] = (float*) malloc(sizeof(float) * 512);
	this->last_fft[AUDIO_FFT_RIGHT] = (float*) malloc(sizeof(float) * 512);
}

/**
 * @brief Get the FFT data if available
 * 
 * @param audio The audio struct
 * @return Pointer to FFT array of last collected data
 */
float** Audio::getFFT(void) {
	// check availability
	if (!(this->fft_l->available() && this->fft_r->available())) {
		return this->last_fft;
	}

	// get data
	for (uint16_t i = 0; i < 512; i++) {
		this->last_fft[AUDIO_FFT_LEFT][i] = this->fft_l->read(i);
		this->last_fft[AUDIO_FFT_RIGHT][i] = this->fft_r->read(i);
	}

	// return the pointer for ease of use
	return this->last_fft;
}

/**
 * @brief Convert an FFT float bin to a integer
 * 
 * @param bin The float value
 * @return uint8_t 0 to 255 for LED control
 */
uint8_t Audio::fftToInt(float bin) { return (uint8_t) round(bin * 255); }
