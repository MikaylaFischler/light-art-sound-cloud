#ifndef LED_CLOUD_MATRIX_HPP_
#define LED_CLOUD_MATRIX_HPP_

#include <inttypes.h>

#include <Adafruit_NeoPixel.h>

namespace LEDCloudMatrix {
	typedef struct led_segment {
		uint8_t strip_id;
		uint16_t offset;
		uint16_t length;
	} led_segment_t;

	enum strip_type { S_R12, S_R34, S_TOP };
	enum seg_o { COLUMN_POS, COLUMN_NEG, ROW_POS, ROW_NEG };
	enum zone { RIGHT, FRONT, LEFT, TOP };

	typedef enum strip_type strip_t;
	typedef enum seg_o seg_o_t;
	typedef enum zone zone_t;

	namespace LEDMath{
		uint8_t quadraticBrightness(uint8_t x);
		uint8_t cubicBrightness(uint8_t x);
		uint8_t quarticBrightness(uint8_t x);

		uint8_t inverseFractional_0745_Brightness(uint8_t x);
		uint8_t inverseFractional_0545_Brightness(uint8_t x);
	};

	namespace LEDColor {
		uint8_t redFromColor(uint32_t color);
		uint8_t greenFromColor(uint32_t color);
		uint8_t blueFromColor(uint32_t color);

		uint32_t color(uint8_t r, uint8_t g, uint8_t b) ;
		uint32_t colorWheel(uint8_t wheel_pos);

		uint32_t scaleColorSaturation(uint32_t color, uint8_t scale);
	};
};

#endif
