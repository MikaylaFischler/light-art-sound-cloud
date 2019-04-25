#include "Animations.hpp"

void Animations::Simple::bassSidePulse(void) {
	unsigned long t_1 = micros();
	Audio::getFFT();
	
	float avg_l = 0;
	for (uint8_t i = 0; i <= FREQ_258HZ; i++) {
		avg_l += Audio::last_fft[AUDIO_FFT_LEFT][i];
	}
	avg_l /= FREQ_258HZ;
	uint8_t out_l = (uint8_t) quadraticBrightness((uint64_t) (avg_l * 6 * 255.0));
	
	float avg_r = 0;
	for (uint8_t i = 0; i <= FREQ_258HZ; i++) {
		avg_r += Audio::last_fft[AUDIO_FFT_RIGHT][i];
	}
	avg_r /= FREQ_258HZ;
	uint8_t out_r = (uint8_t) quadraticBrightness((uint64_t) (avg_r * 6 * 255.0));

	lcm->setZone(zone_t::LEFT, out_l, 0, 0);
	lcm->setZone(zone_t::RIGHT, out_r, 0, 0);
	lcm->showZone(zone_t::LEFT); // will show both

	unsigned long t_2 = micros();
	Serial.println(t_2 - t_1);
}

void Animations::Simple::bassRangePulse(void) {
	unsigned long t_1 = micros();
	Audio::getFFT();
	
	float avg_l_sb = 0;
	float avg_r_sb = 0;
	
	float avg_l_b = 0;
	float avg_r_b = 0;
	
	float avg_l_lm = 0;
	float avg_r_lm = 0;
	
	for (uint8_t i = 0; i <= FREQ_86HZ; i++) {
		avg_l_sb += Audio::last_fft[AUDIO_FFT_LEFT][i];
		avg_r_sb += Audio::last_fft[AUDIO_FFT_RIGHT][i];
	}

	avg_l_sb /= FREQ_86HZ;
	avg_r_sb /= FREQ_86HZ;
	
	for (uint8_t i = FREQ_129HZ; i <= FREQ_258HZ; i++) {
		avg_l_b += Audio::last_fft[AUDIO_FFT_LEFT][i];
		avg_r_b += Audio::last_fft[AUDIO_FFT_RIGHT][i];
	}

	avg_l_b /= FREQ_258HZ - FREQ_129HZ + 1;
	avg_r_b /= FREQ_258HZ - FREQ_129HZ + 1;
	
	for (uint8_t i = FREQ_258HZ; i <= FREQ_516HZ; i++) {
		avg_l_lm += Audio::last_fft[AUDIO_FFT_LEFT][i];
		avg_r_lm += Audio::last_fft[AUDIO_FFT_RIGHT][i];
	}

	avg_l_lm /= FREQ_516HZ - FREQ_258HZ + 1;
	avg_r_lm /= FREQ_516HZ - FREQ_258HZ + 1;

	uint8_t out_l_sb = (uint8_t) cubicBrightness((uint64_t) (avg_l_sb * 6 * 255.0));
	uint8_t out_r_sb = (uint8_t) cubicBrightness((uint64_t) (avg_r_sb * 6 * 255.0));

	uint8_t out_l_b = (uint8_t) cubicBrightness((uint64_t) (avg_l_b * 6 * 255.0));
	uint8_t out_r_b = (uint8_t) cubicBrightness((uint64_t) (avg_r_b * 6 * 255.0));

	uint8_t out_l_lm = (uint8_t) cubicBrightness((uint64_t) (avg_l_lm * 6 * 255.0));
	uint8_t out_r_lm = (uint8_t) cubicBrightness((uint64_t) (avg_r_lm * 6 * 255.0));


	lcm->setZone(zone_t::LEFT, out_l_sb, out_l_sb, 0);
	lcm->setZone(zone_t::RIGHT, out_r_sb, out_r_sb, 0);

	for (int i = 11; i <= 13; i++) {
		lcm->setZoneCol(zone_t::TOP, i, out_l_b, out_l_b, out_l_b);
	}

	for (int i = 0; i <= 3; i++) {
		lcm->setZoneCol(zone_t::TOP, i, out_r_b, out_r_b, out_r_b);
		lcm->setZoneCol(zone_t::FRONT, i, 0, out_r_lm, 0);
	}

	for (int i = 9; i <= 12; i++) {
		lcm->setZoneCol(zone_t::FRONT, i, 0, out_r_lm, 0);
	}

	lcm->show();

	unsigned long t_2 = micros();
	Serial.println(t_2 - t_1);
}

void Animations::Simple::bassTreblePulse(void) {
	unsigned long t_1 = micros();
	Audio::getFFT();
	
	float avg_l_sb, avg_r_sb = 0;
	float avg_l_b, avg_r_b = 0;
	float avg_l_lm, avg_r_lm = 0;
	float avg_l_m, avg_r_m = 0;
	float avg_l_hm, avg_r_hm = 0;
	float avg_p = 0;
	
	for (uint8_t i = 0; i <= FREQ_86HZ; i++) {
		avg_l_sb += Audio::last_fft[AUDIO_FFT_LEFT][i];
		avg_r_sb += Audio::last_fft[AUDIO_FFT_RIGHT][i];
	}

	avg_l_sb /= FREQ_86HZ;
	avg_r_sb /= FREQ_86HZ;
	
	for (uint8_t i = FREQ_129HZ; i <= FREQ_258HZ; i++) {
		avg_l_b += Audio::last_fft[AUDIO_FFT_LEFT][i];
		avg_r_b += Audio::last_fft[AUDIO_FFT_RIGHT][i];
	}

	avg_l_b /= FREQ_258HZ - FREQ_129HZ + 1;
	avg_r_b /= FREQ_258HZ - FREQ_129HZ + 1;
	
	for (uint8_t i = FREQ_258HZ; i <= FREQ_516HZ; i++) {
		avg_l_lm += Audio::last_fft[AUDIO_FFT_LEFT][i];
		avg_r_lm += Audio::last_fft[AUDIO_FFT_RIGHT][i];
	}

	avg_l_lm /= FREQ_516HZ - FREQ_258HZ + 1;
	avg_r_lm /= FREQ_516HZ - FREQ_258HZ + 1;
	
	for (uint8_t i = FREQ_516HZ; i <= FREQ_2021HZ; i++) {
		avg_l_m += Audio::last_fft[AUDIO_FFT_LEFT][i];
		avg_r_m += Audio::last_fft[AUDIO_FFT_RIGHT][i];
	}

	avg_l_m /= 15;
	avg_r_m /= 15;
	
	for (uint8_t i = FREQ_2021HZ; i <= FREQ_3999HZ; i++) {
		avg_l_hm += Audio::last_fft[AUDIO_FFT_LEFT][i];
		avg_r_hm += Audio::last_fft[AUDIO_FFT_RIGHT][i];
	}

	avg_l_hm /= 6;
	avg_r_hm /= 6;
	
	for (uint8_t i = FREQ_4042HZ; i <= FREQ_6020HZ; i++) {
		avg_p += Audio::last_fft[AUDIO_FFT_LEFT][i];
		avg_p += Audio::last_fft[AUDIO_FFT_RIGHT][i];
	}

	avg_p /= 12;

	uint8_t out_l_sb = (uint8_t) cubicBrightness((uint64_t) (avg_l_sb * 6 * 255.0));
	uint8_t out_r_sb = (uint8_t) cubicBrightness((uint64_t) (avg_r_sb * 6 * 255.0));

	uint8_t out_l_b = (uint8_t) cubicBrightness((uint64_t) (avg_l_b * 6 * 255.0));
	uint8_t out_r_b = (uint8_t) cubicBrightness((uint64_t) (avg_r_b * 6 * 255.0));

	uint8_t out_l_lm = (uint8_t) cubicBrightness((uint64_t) (avg_l_lm * 6 * 255.0));
	uint8_t out_r_lm = (uint8_t) cubicBrightness((uint64_t) (avg_r_lm * 6 * 255.0));

	uint8_t out_l_m = (uint8_t) cubicBrightness((uint64_t) (avg_l_m * 6 * 255.0));
	uint8_t out_r_m = (uint8_t) cubicBrightness((uint64_t) (avg_r_m * 6 * 255.0));

	uint8_t out_l_hm = (uint8_t) cubicBrightness((uint64_t) (avg_l_hm * 6 * 255.0));
	uint8_t out_r_hm = (uint8_t) cubicBrightness((uint64_t) (avg_r_hm * 6 * 255.0));

	uint8_t out_p = (uint8_t) cubicBrightness((uint64_t) (avg_p * 6 * 255.0));

	lcm->setZone(zone_t::LEFT, out_l_sb, out_l_sb, 0);
	lcm->setZone(zone_t::RIGHT, out_r_sb, out_r_sb, 0);

	for (int i = 10; i <= 13; i++) {
		lcm->setZoneCol(zone_t::TOP, i, out_l_b, out_l_b, out_l_b);
	}

	for (int i = 4; i <= 6; i++) {
		lcm->setZoneCol(zone_t::TOP, i, out_l_hm / 2, 0, out_l_hm);
	}

	for (int i = 7; i <= 9; i++) {
		lcm->setZoneCol(zone_t::TOP, i, out_r_hm / 2, 0, out_r_hm);
	}

	for (int i = 0; i <= 3; i++) {
		lcm->setZoneCol(zone_t::TOP, i, out_r_b, out_r_b, out_r_b);
		lcm->setZoneCol(zone_t::FRONT, i, 0, out_r_lm, 0);
	}

	for (int i = 9; i <= 12; i++) {
		lcm->setZoneCol(zone_t::FRONT, i, 0, out_r_lm, 0);
	}

	for (int i = 7; i <= 8; i++) {
		lcm->setZoneCol(zone_t::FRONT, i, 0, out_l_m, out_l_m);
	}

	for (int i = 4; i <= 5; i++) {
		lcm->setZoneCol(zone_t::FRONT, i, 0, out_r_m, out_r_m);
	}

	lcm->setZoneRow(zone_t::FRONT, 3, out_p, 0, 0);

	lcm->show();

	unsigned long t_2 = micros();
	// Serial.println(out_r_hm);
}
