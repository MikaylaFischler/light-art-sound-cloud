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
#define AUDIO_FFT_RIGHT 0

class Audio {
public:
	AudioInputI2S* board;
	AudioAnalyzeFFT1024* fft_l;
	AudioAnalyzeFFT1024* fft_r;
	AudioConnection* fft_l_conn;
	AudioConnection* fft_r_conn;
	float** last_fft;

	Audio(void);
	~Audio(void) {};

	float** getFFT(void);
	uint8_t fftToInt(float bin);
};

// AudioAnalyzeRMS rms1;
// AudioAnalyzePeak peak1;
// AudioAnalyzeNoteFrequency notefreq1;
// AudioAnalyzeToneDetect tone1;

#endif
