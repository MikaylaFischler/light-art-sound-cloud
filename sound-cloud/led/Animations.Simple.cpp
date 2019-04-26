#include "Animations.hpp"

/**
 * @brief Pulse the left and right side in red with the left and right bass ranges, respectively
 * 
 */
void Animations::Simple::bassSidePulse(void) {
	unsigned long t_1 = micros();
	Audio::getFFTWhenReadyBlocking();

	float* avgs = Audio::averageDualFFTRange(FREQ_0HZ, FREQ_258HZ);

	uint8_t out_l = Audio::fftToInt(avgs[0], 6, quadraticBrightness);
	uint8_t out_r = Audio::fftToInt(avgs[1], 6, quadraticBrightness);

	lcm->setZone(zone_t::LEFT, out_l, 0, 0);
	lcm->setZone(zone_t::RIGHT, out_r, 0, 0);
	lcm->showZone(zone_t::LEFT); // will show both

	unsigned long t_2 = micros();
	Serial.println(t_2 - t_1);
}

/**
 * @brief Pulse the sides, top, and front with different bass ranges.
 * 
 */
void Animations::Simple::bassRangePulse(void) {
	unsigned long t_1 = micros();
	Audio::getFFTWhenReadyBlocking();

	float* avgs_sb = Audio::averageDualFFTRange(FREQ_0HZ, FREQ_86HZ);
	float* avgs_b = Audio::averageDualFFTRange(FREQ_129HZ, FREQ_258HZ);
	float* avgs_lm = Audio::averageDualFFTRange(FREQ_258HZ, FREQ_516HZ);

	uint8_t out_l_sb = Audio::fftToInt(avgs_sb[0], 6, quadraticBrightness);
	uint8_t out_r_sb = Audio::fftToInt(avgs_sb[1], 6, quadraticBrightness);

	uint8_t out_l_b = Audio::fftToInt(avgs_b[0], 6, quadraticBrightness);
	uint8_t out_r_b = Audio::fftToInt(avgs_b[1], 6, quadraticBrightness);

	uint8_t out_l_lm = Audio::fftToInt(avgs_lm[0], 6, quadraticBrightness);
	uint8_t out_r_lm = Audio::fftToInt(avgs_lm[1], 6, quadraticBrightness);

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
		lcm->setZoneCol(zone_t::FRONT, i, 0, out_l_lm, 0);
	}

	lcm->show();

	unsigned long t_2 = micros();
	Serial.println(t_2 - t_1);
}

/**
 * @brief Render sub-bass through high mids across the full cloud
 * 
 */
void Animations::Simple::bassTreblePulse(void) {
	unsigned long t_1 = micros();
	// Audio::enableHysteresis();
	Audio::getFFTWhenReadyBlocking();

	float* avgs_sb = Audio::averageDualFFTRange(FREQ_0HZ, FREQ_86HZ);
	float* avgs_b = Audio::averageDualFFTRange(FREQ_129HZ, FREQ_258HZ);
	float* avgs_lm = Audio::averageDualFFTRange(FREQ_258HZ, FREQ_516HZ);
	float* avgs_m = Audio::averageDualFFTRangeUnbalanced(FREQ_516HZ, FREQ_2021HZ, 15);
	float* avgs_hm = Audio::averageDualFFTRangeUnbalanced(FREQ_2021HZ, FREQ_3999HZ, 6);
	float avg_p = Audio::averageFFTRangeUnbalanced(AUDIO_FFT_COMBINED, FREQ_2021HZ, FREQ_3999HZ, 12);

	uint8_t out_l_sb = Audio::fftToInt(avgs_sb[0], 6, cubicBrightness);
	uint8_t out_r_sb = Audio::fftToInt(avgs_sb[1], 6, cubicBrightness);

	uint8_t out_l_b = Audio::fftToInt(avgs_b[0], 6, cubicBrightness);
	uint8_t out_r_b = Audio::fftToInt(avgs_b[1], 6, cubicBrightness);

	uint8_t out_l_lm = Audio::fftToInt(avgs_lm[0], 6, cubicBrightness);
	uint8_t out_r_lm = Audio::fftToInt(avgs_lm[1], 6, cubicBrightness);

	uint8_t out_l_m = Audio::fftToInt(avgs_m[0], 6, cubicBrightness);
	uint8_t out_r_m = Audio::fftToInt(avgs_m[1], 6, cubicBrightness);

	uint8_t out_l_hm = Audio::fftToInt(avgs_hm[0], 6, cubicBrightness);
	uint8_t out_r_hm = Audio::fftToInt(avgs_hm[1], 6, cubicBrightness);

	uint8_t out_p = Audio::fftToInt(avg_p, 6, cubicBrightness);

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
		lcm->setZoneCol(zone_t::FRONT, i, 0, out_l_lm, 0);
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
	Serial.println(t_2 - t_1);
}
