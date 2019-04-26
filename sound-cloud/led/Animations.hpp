#ifndef CLOUD_ANIMATIONS_HPP_
#define CLOUD_ANIMATIONS_HPP_

#include <inttypes.h>

#include "color.h"
#include "../audio/Audio.hpp"
#include "LEDCloudMatrix.hpp"
#include "LEDCloudControl.hpp"

using namespace LEDCloudMatrix;
using namespace LEDCloudMatrix::LEDMath;
using namespace LEDCloudMatrix::LEDColor;

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
				static void bassSidePulse(void);
				static void bassRangePulse(void);
				static void bassTreblePulse(void);
				// static void fullRangeDisplay(void);
				// static void noteFlash(void);
		};

		class Advanced {
			public:
				static void solidLinearVisualizer3D(void);
				static void rainbowLinearVisualizer3D(void);
				static void fireLinearVisualizer3D(void);
				// static void bassFlow(void);
		};
	private:
		Animations() {};
		virtual ~Animations() {};
	
		static LEDCloudControl* lcm;

		friend class Animations::Standby;
		friend class Animations::Simple;
		friend class Animations::Advanced;
};

#endif
