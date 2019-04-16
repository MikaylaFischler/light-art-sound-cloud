#ifndef CLOUD_ANIMATIONS_HPP_
#define CLOUD_ANIMATIONS_HPP_

#include "../audio/Audio.hpp"
#include "LEDCloudMatrix.hpp"

class Animations {
	public:
		static void linkCloudMatrix(LEDCloudMatrix* lcm);
		static void linKAudio(Audio* audio);

		// basic functions
		static void set_AllOff(void);

	private:
		Animations() {};
		virtual ~Animations() {};

		static LEDCloudMatrix* lcm;
		static Audio* audio;
};

#endif
