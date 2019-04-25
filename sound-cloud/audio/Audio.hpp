#ifndef CLOUD_AUDIO_HPP_
#define CLOUD_AUDIO_HPP_

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <inttypes.h>
#include "freq_map.h"

#define AUDIO_FFT_LEFT 0
#define AUDIO_FFT_RIGHT 1
#define AUDIO_FFT_COMBINED 2

class Audio {
public:
	static AudioControlSGTL5000* board;
	static AudioInputI2S* input;
	static AudioAnalyzeFFT1024* fft_l;
	static AudioAnalyzeFFT1024* fft_r;
	static AudioConnection* fft_l_conn;
	static AudioConnection* fft_r_conn;
	static float** last_fft;

	static void init(void);

	static float** getFFT(void);
	static float** getFFTWhenReady(void);
	static float** getFFTWhenReadyBlocking(void);

	static float averageFFTRange(uint8_t side, uint16_t bin_start, uint16_t bin_end);
	static float averageFFTRangeUnbalanced(uint8_t side, uint16_t bin_start, uint16_t bin_end, float division_factor);
	
	static float* averageDualFFTRange(uint16_t bin_start, uint16_t bin_end);
	static float* averageDualFFTRangeUnbalanced(uint16_t bin_start, uint16_t bin_end, float division_factor);

	static uint8_t fftToInt(float bin, float scale_factor, uint8_t (*brightness_transform)(uint8_t));
private:
	static uint8_t ready;
	Audio(void) {};
	~Audio(void) {};
};

// AudioAnalyzeRMS rms1;
// AudioAnalyzePeak peak1;
// AudioAnalyzeNoteFrequency notefreq1;
// AudioAnalyzeToneDetect tone1;

#endif
