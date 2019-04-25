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

	// get data
	for (uint16_t i = 0; i < 512; i++) {
		last_fft[AUDIO_FFT_LEFT][i] = fft_l->read(i);
		last_fft[AUDIO_FFT_RIGHT][i] = fft_r->read(i);
	}

	// return the pointer for ease of use
	return last_fft;
}

/**
 * @brief Convert an FFT float bin to a integer
 * 
 * @param bin The float value
 * @return uint8_t 0 to 255 for LED control
 */
uint8_t Audio::fftToInt(float bin) { return (uint8_t) round(bin * 255); }
