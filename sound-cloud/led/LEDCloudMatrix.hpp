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

	class LEDCloudControl {
	public:
		LEDCloudControl(Adafruit_NeoPixel* row_1_2_strip, Adafruit_NeoPixel* row_3_4_strip, Adafruit_NeoPixel* top_strip);
		~LEDCloudControl();

		Adafruit_NeoPixel* strip(strip_t type) const;
		uint16_t levelLength(uint8_t level) const;
		uint16_t zoneWidth(zone_t zone_id) const;
		uint16_t zoneHeight(zone_t zone_id) const;

		led_segment_t* createSegment(strip_t strip, uint8_t start, uint8_t length);
		void mapSegmentToZone(led_segment_t* segment, zone_t zone_id, uint8_t row, uint8_t col, seg_o_t orientation);
		void mapSegmentToLevel(led_segment_t* segment, uint8_t level);
		
		void setPixelColorByZone(zone_t zone_id, uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
		void setPixelColorByZone(zone_t zone_id, uint8_t x, uint8_t y, uint32_t c);
		void setPixelColorByLevel(uint8_t level, uint16_t n, uint8_t r, uint8_t g, uint8_t b);
		void setPixelColorByLevel(uint8_t level, uint16_t n, uint32_t c);

		void show(void);
		void showLevel(uint8_t level);
		void showZone(zone_t zone_id);
	private:
		Adafruit_NeoPixel* raw_strips[3];
		
		void extend_level(void);
		void extend_zone(zone_t zone_id);

		struct px_map {
			uint8_t strip_id;
			uint16_t offset;
		};

		struct px_level {
			uint8_t strip_id;
			uint16_t offset;
			uint16_t length;
		};

		struct px_map_matrix {
			uint16_t dim;			//! max square dimension before array growth needed
			uint16_t r_w;			//! real width
			uint16_t r_h;			//! real height
			struct px_map** map;	//! 2D array
		};

		struct px_map_matrix zones[4];

		struct px_map_arr {
			uint16_t dim;			//! max dimension before array growth needed
			uint16_t r_dim;			//! real number of levels
			struct px_level* map;	//! 1D array
		} levels;
	};
};

#endif
