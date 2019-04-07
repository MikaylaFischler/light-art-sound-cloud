#include "LEDCloudMatrix.hpp"

const uint16_t map__level[] = {
	// level 0
	0,
	// level 1
	20, // TODO place holder
	// level 2
	40 // TODO place holder
	// etc
};

const uint16_t map__full_matrix[][1] = {
	{0}, {0} // TODO do this
};

const uint16_t map__top_matrix[][1] = {
	{0}, {0} // TODO do this
};

LEDCloudMatrix::LEDCloudMatrix(Adafruit_NeoPixel* strip) {
	this->raw_strip = strip;
	this->raw_strip->setBrightness(75);
}

Adafruit_NeoPixel* LEDCloudMatrix::strip(void) { return this->raw_strip; }

void LEDCloudMatrix::setPixelColorByFullMatrix(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
	this->raw_strip->setPixelColor(map__full_matrix[x][y], r, g, b);
}

void LEDCloudMatrix::setPixelColorByFullMatrix(uint8_t x, uint8_t y, uint32_t c) {
	this->raw_strip->setPixelColor(map__full_matrix[x][y], c);
}

void LEDCloudMatrix::setPixelColorByTopMatrix(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
	this->raw_strip->setPixelColor(map__top_matrix[x][y], r, g, b);
}

void LEDCloudMatrix::setPixelColorByTopMatrix(uint8_t x, uint8_t y, uint32_t c) {
	this->raw_strip->setPixelColor(map__top_matrix[x][y], c);
}

void LEDCloudMatrix::setPixelColorByLevel(uint8_t level, uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
	this->raw_strip->setPixelColor(map__level[level] + n, r, g, b);
}

void LEDCloudMatrix::setPixelColorByLevel(uint8_t level, uint16_t n, uint32_t c) {
	this->raw_strip->setPixelColor(map__level[level] + n, c);
}
