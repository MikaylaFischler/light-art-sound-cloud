#ifndef LED_CLOUD_MATRIX_HPP_
#define LED_CLOUD_MATRIX_HPP_

#include <inttypes.h>

#include <Adafruit_NeoPixel.h>

typedef struct led_segment {
	uint8_t id;
	uint16_t offset;
	uint16_t length;
} led_segment_t;

class LEDCloudMatrix {
public:
	static enum { ROW_1_2, ROW_3_4, TOP } Type;

	LEDCloudMatrix(Adafruit_NeoPixel* row_1_2_strip, Adafruit_NeoPixel* row_3_4_strip, Adafruit_NeoPixel* top_strip);
	~LEDCloudMatrix() {};

	Adafruit_NeoPixel* strip(uint8_t type) const;
	uint16_t levelLength(uint8_t level) const;
	uint16_t fullMatrixWidth(void) const;
	uint16_t fullMatrixHeight(void) const;
	uint16_t topMatrixWidth(void) const;
	uint16_t topMatrixHeight(void) const;

	led_segment_t* createSegment(uint8_t strip, uint8_t start, uint8_t length);
	void mapSegmentToFullMatrix(led_segment_t* segment, uint8_t row, uint8_t col);
	void mapSegmentToTopMatrix(led_segment_t* segment, uint8_t row, uint8_t col);
	void mapSegmentToLevel(led_segment_t* segment, uint8_t level);
	
	void setPixelColorByFullMatrix(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
	void setPixelColorByFullMatrix(uint8_t x, uint8_t y, uint32_t c);
	void setPixelColorByTopMatrix(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
	void setPixelColorByTopMatrix(uint8_t x, uint8_t y, uint32_t c);
	void setPixelColorByLevel(uint8_t level, uint16_t n, uint8_t r, uint8_t g, uint8_t b);
	void setPixelColorByLevel(uint8_t level, uint16_t n, uint32_t c);
private:
	Adafruit_NeoPixel* raw_strips[3];
	
	uint8_t next_seg_id;

	void extend_map__full(void);
	void extend_map__top(void);
	void extend_map__level(void);

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
		uint16_t r_x;			//! real max x
		uint16_t r_y;			//! real max y
		struct px_map** map;	//! 2D array
	} map__full_matrix, map__top_matrix;

	struct px_map_arr {
		uint16_t dim;			//! max dimension before array growth needed
		uint16_t r_count;		//! real number of levels
		struct px_level* map;	//! 1D array
	} map__level;
};

#endif
