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

	static uint8_t redFromColor(uint32_t color);
	static uint8_t greenFromColor(uint32_t color);
	static uint8_t blueFromColor(uint32_t color);

	static uint32_t color(uint8_t r, uint8_t g, uint8_t b) ;
	static uint32_t colorWheel(uint8_t wheel_pos);
};

#endif
