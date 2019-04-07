#include "audio.hpp"

/**
 * @brief Create an audio struct
 * @return Pointer to audio struct
 */
struct audio* audio_create(void) {
	struct audio* audio = (struct audio*) malloc(sizeof(struct audio));
}

/**
 * @brief Initialize an audio struct
 * @param audio Pointer to audio struct
 */
void audio_init(struct audio* audio) {
	// create objects for audio processing
	audio->board = new AudioInputI2S();
	audio->fft_l = new AudioAnalyzeFFT1024();
	audio->fft_r = new AudioAnalyzeFFT1024();
	audio->fft_l_conn = new AudioConnection(*audio->fft_l, 1, *audio->fft_l, 0);
	audio->fft_r_conn = new AudioConnection(*audio->fft_r, 0, *audio->fft_r, 0);

	// last FFT data set
	audio->last_fft = (float**) malloc(sizeof(float*) * 2);
	audio->last_fft[AUDIO_FFT_LEFT] = (float*) malloc(sizeof(float) * 512);
	audio->last_fft[AUDIO_FFT_RIGHT] = (float*) malloc(sizeof(float) * 512);
}

/**
 * @brief Get the FFT data if available
 * @param audio The audio struct
 * @return Pointer to FFT array or NULL if was not ready
 */
float** audio_get_fft(struct audio* audio) {
	// check availability
	if (!(audio->fft_l->available() && audio->fft_r->available())) {
		return NULL;
	}

	// get data
	for (uint16_t i = 0; i < 512; i++) {
		audio->last_fft[AUDIO_FFT_LEFT][i] = audio->fft_l->read(i);
		audio->last_fft[AUDIO_FFT_RIGHT][i] = audio->fft_r->read(i);
	}

	// return the pointer for ease of use
	return audio->last_fft;
}
