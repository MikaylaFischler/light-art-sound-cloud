#ifndef CLOUD_ANIMATIONS_HPP_
#define CLOUD_ANIMATIONS_HPP_

#include "color.h"
#include "../audio/Audio.hpp"
#include "LEDCloudMatrix.hpp"

using namespace LEDCloudMatrix;

class Animations {
	public:
		static void linkCloudControl(LEDCloudControl* lcm);

		// basic functions
		static void allOff(void);
		static void diagnostics(void);

		class Standby {
			public:
				static void solidSoftWhite(void);
				static void whiteWave(void);
				static void cloudToCloudLighting(void);
				static void rainbowWash(void);
				static void rainbowFade(void);
		};

		class Simple {
			public:
				// static void bassRangePulse(void);
				// static void bassTreblePulse(void);
				// static void noteFlash(void);
				// static void linearVisualizer3D(void);
		};

		class Complex {
			public:
				// static void bassFlow(void);
		};
	private:
		Animations() {};
		virtual ~Animations() {};
	
		static LEDCloudControl* lcm;

		friend class Animations::Standby;
		friend class Animations::Simple;
		friend class Animations::Complex;
};

#endif
