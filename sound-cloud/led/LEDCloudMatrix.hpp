#ifndef LED_CLOUD_MATRIX_HPP_
#define LED_CLOUD_MATRIX_HPP_

#include <inttypes.h>

#include <Adafruit_NeoPixel.h>

class LEDCloudMatrix {
public:
	LEDCloudMatrix(Adafruit_NeoPixel* strip);
	~LEDCloudMatrix() {};

	Adafruit_NeoPixel* strip(void);
	
	void setPixelColorByFullMatrix(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
	void setPixelColorByFullMatrix(uint8_t x, uint8_t y, uint32_t c);
	void setPixelColorByTopMatrix(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
	void setPixelColorByTopMatrix(uint8_t x, uint8_t y, uint32_t c);
	void setPixelColorByLevel(uint8_t level, uint16_t n, uint8_t r, uint8_t g, uint8_t b);
	void setPixelColorByLevel(uint8_t level, uint16_t n, uint32_t c);
private:
	Adafruit_NeoPixel* raw_strip;
};

#endif
