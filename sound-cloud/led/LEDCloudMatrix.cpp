#include "LEDCloudMatrix.hpp"

/**
 * @brief Constructor for a Cloud Matrix
 * 
 * @param row_1_2_strip The strip for rows 1 and 2
 * @param row_3_4_strip The strip for rows 3 and 4
 * @param top_strip The strip for the top LEDs
 */
LEDCloudMatrix::LEDCloudMatrix(Adafruit_NeoPixel* row_1_2_strip, Adafruit_NeoPixel* row_3_4_strip, Adafruit_NeoPixel* top_strip) {
	// store strips
	this->raw_strips[LEDCloudMatrix::ROW_1_2] = row_1_2_strip;
	this->raw_strips[LEDCloudMatrix::ROW_3_4] = row_3_4_strip;
	this->raw_strips[LEDCloudMatrix::TOP] = top_strip;
	
	// default brightness
	for (uint8_t i = 0; i < 3; i++) {
		this->raw_strips[i]->setBrightness(75);
		this->raw_strips[i]->show();
	}

	// zero segment ID start
	next_seg_id = 0;

	// allocate maps
	this->map__full_matrix.dim = 50;
	this->map__full_matrix.r_x = this->map__full_matrix.r_y = 0;
	this->map__full_matrix.map = (struct px_map**) malloc(sizeof(struct px_map*) * this->map__full_matrix.dim);
	for (uint8_t i = 0; i < this->map__full_matrix.dim; i++) {
		this->map__full_matrix.map[i] = (struct px_map*) malloc(sizeof(struct px_map) * this->map__full_matrix.dim);
		for (uint8_t j = 0; j < this->map__full_matrix.dim; j++) {
			this->map__full_matrix.map[i][j].strip_id = -1;	// mark invalid
		}
	}

	this->map__top_matrix.dim = 20;
	this->map__top_matrix.r_x = this->map__top_matrix.r_y = 0;
	this->map__top_matrix.map = (struct px_map**) malloc(sizeof(struct px_map*) * this->map__top_matrix.dim);
	for (uint8_t i = 0; i < this->map__top_matrix.dim; i++) {
		this->map__top_matrix.map[i] = (struct px_map*) malloc(sizeof(struct px_map) * this->map__top_matrix.dim);
		for (uint8_t j = 0; j < this->map__top_matrix.dim; j++) {
			this->map__top_matrix.map[i][j].strip_id = -1;	// mark invalid
		}
	}

	this->map__level.dim = 5;
	this->map__level.r_count = 0;
	this->map__level.map = (struct px_level*) malloc(sizeof(struct px_level) * this->map__level.dim);
	for (uint8_t i = 0; i < this->map__top_matrix.dim; i++) {
		this->map__level.map[i].strip_id = -1;	// mark invalid
	}
}

/**
 * @brief Destroy the LEDCloudMatrix::LEDCloudMatrix object
 * @note Frees the three map matricies/arrays
 */
LEDCloudMatrix::~LEDCloudMatrix() {
	// free full matrix
	for (uint8_t i = 0; i < this->map__full_matrix.dim; i++) { free(this->map__full_matrix.map[i]); }
	free(this->map__full_matrix.map);

	// free top matrix
	for (uint8_t i = 0; i < this->map__top_matrix.dim; i++) { free(this->map__top_matrix.map[i]); }
	free(this->map__top_matrix.map);

	// free level map
	free(this->map__level.map);
}

/**
 * @brief Get a raw Adafruit_NeoPixel strip by ID
 * 
 * @param id Strip ID
 * @return Adafruit_NeoPixel* selected strip
 */
Adafruit_NeoPixel* LEDCloudMatrix::strip(uint8_t id) const { return this->raw_strips[id]; }

/**
 * @brief Get the length of a particular level
 * 
 * @param level The level
 * @return uint16_t That level's length
 */
uint16_t LEDCloudMatrix::levelLength(uint8_t level) const { return this->map__level.r_count; }

/**
 * @brief Get the width of the full matrix
 * 
 * @return uint16_t Width
 */
uint16_t LEDCloudMatrix::fullMatrixWidth(void) const { return this->map__full_matrix.r_x; }

/**
 * @brief Get the height of the full matrix
 * 
 * @return uint16_t Height
 */
uint16_t LEDCloudMatrix::fullMatrixHeight(void) const { return this->map__full_matrix.r_y; }

/**
 * @brief Get the width of the top matrix
 * 
 * @return uint16_t Width
 */
uint16_t LEDCloudMatrix::topMatrixWidth(void) const { return this->map__top_matrix.r_x; }

/**
 * @brief Get the height of the top matrix
 * 
 * @return uint16_t Height
 */
uint16_t LEDCloudMatrix::topMatrixHeight(void) const { return this->map__top_matrix.r_y; }

/**
 * @brief Create a segment from a strip
 * 
 * @param strip The strip ID
 * @param start The start position
 * @param length The length
 * @return led_segment_t* The new segment object
 */
led_segment_t* LEDCloudMatrix::createSegment(uint8_t strip, uint8_t start, uint8_t length) {
	led_segment_t* seg = (led_segment_t*) malloc(sizeof(led_segment_t));
	seg->id = next_seg_id++;
	seg->offset = start;
	seg->length = length;
	return seg;
}

/**
 * @brief Map a segment to the full matrix
 * 
 * @param segment The segment to map
 * @param row The starting row
 * @param col The starting column
 * @param orientation The orientation for the segment in the matrix
 */
void LEDCloudMatrix::mapSegmentToFullMatrix(led_segment_t* segment, uint8_t row, uint8_t col, enum seg_o orientation) {
	const uint16_t row_max = row + segment->length;
	const uint16_t col_max = col + segment->length;

	// make sure we can fit this and update the real lengths if necessary
	while (this->map__full_matrix.dim >= row_max || this->map__full_matrix.dim >= col_max) { extend_map__full(); }
	if (this->map__full_matrix.r_x < col_max) { this->map__full_matrix.r_x = col_max; }
	if (this->map__full_matrix.r_y < row_max) { this->map__full_matrix.r_y = row_max; }

	// map the segment
	if (orientation == COLUMN) {
		for (uint16_t r = 0; r < segment->length; r++) {
			this->map__full_matrix.map[row + r][col].strip_id = segment->id;
			this->map__full_matrix.map[row + r][col].offset = segment->offset + r;
		}
	} else {
		for (uint16_t c = 0; c < segment->length; c++) {
			this->map__full_matrix.map[row][col + c].strip_id = segment->id;
			this->map__full_matrix.map[row][col + c].offset = segment->offset + c;
		}
	}
}

/**
 * @brief Map a segment to the top matrix
 * 
 * @param segment The segment to map
 * @param row The starting row
 * @param col The starting column
 * @param orientation The orientation for the segment in the matrix
 */
void LEDCloudMatrix::mapSegmentToTopMatrix(led_segment_t* segment, uint8_t row, uint8_t col, enum seg_o orientation) {
	const uint16_t row_max = row + segment->length;
	const uint16_t col_max = col + segment->length;

	// make sure we can fit this and update the real lengths if necessary
	while (this->map__top_matrix.dim >= row_max || this->map__top_matrix.dim >= col_max) { extend_map__top(); }
	if (this->map__top_matrix.r_x < col_max) { this->map__top_matrix.r_x = col_max; }
	if (this->map__top_matrix.r_y < row_max) { this->map__top_matrix.r_y = row_max; }

	// map the segment
	if (orientation == COLUMN) {
		for (uint16_t r = 0; r < segment->length; r++) {
			this->map__top_matrix.map[row + r][col].strip_id = segment->id;
			this->map__top_matrix.map[row + r][col].offset = segment->offset + r;
		}
	} else {
		for (uint16_t c = 0; c < segment->length; c++) {
			this->map__top_matrix.map[row][col + c].strip_id = segment->id;
			this->map__top_matrix.map[row][col + c].offset = segment->offset + c;
		}
	}
}

/**
 * @brief Map a segment to a level
 * 
 * @param segment The segment to map
 * @param level The level to map to
 */
void LEDCloudMatrix::mapSegmentToLevel(led_segment_t* segment, uint8_t level) {
	// make sure we can fit this and update the real length if necessary
	while (this->map__level.dim >= level) { extend_map__level(); }
	if (this->map__level.r_count < level + 1) { this->map__level.r_count = level + 1; }

	// set the map map
	this->map__level.map[level].strip_id = segment->id;
	this->map__level.map[level].offset = segment->offset;
}

/**
 * @brief Set a pixel color via the full matrix
 * 
 * @param x X coordinate (column)
 * @param y Y coordinate (row)
 * @param r Red value
 * @param g Green value
 * @param b Blue value
 */
void LEDCloudMatrix::setPixelColorByFullMatrix(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
	struct px_map* m = &(this->map__full_matrix.map[x][y]);
	this->raw_strips[m->strip_id]->setPixelColor(m->offset, r, g, b);
}

/**
 * @brief Set a pixel color via the full matrix
 * 
 * @param x X coordinate (column)
 * @param y Y coordinate (row)
 * @param c 32-bit color
 */
void LEDCloudMatrix::setPixelColorByFullMatrix(uint8_t x, uint8_t y, uint32_t c) {
	struct px_map* m = &(this->map__full_matrix.map[x][y]);
	this->raw_strips[m->strip_id]->setPixelColor(m->offset, c);
}

/**
 * @brief Set a pixel color via the top matrix
 * 
 * @param x X coordinate (column)
 * @param y Y coordinate (row)
 * @param r Red value
 * @param g Green value
 * @param b Blue value
 */
void LEDCloudMatrix::setPixelColorByTopMatrix(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
	struct px_map* m = &(this->map__full_matrix.map[x][y]);
	this->raw_strips[m->strip_id]->setPixelColor(m->offset, r, g, b);
}

/**
 * @brief Set a pixel color via the top matrix
 * 
 * @param x X coordinate (column)
 * @param y Y coordinate (row)
 * @param c 32-bit color
 */
void LEDCloudMatrix::setPixelColorByTopMatrix(uint8_t x, uint8_t y, uint32_t c) {
	struct px_map* m = &(this->map__top_matrix.map[x][y]);
	this->raw_strips[m->strip_id]->setPixelColor(m->offset, c);
}

/**
 * @brief Set a pixel color via the level
 * 
 * @param level The level to write to
 * @param x N pixels from start of level
 * @param r Red value
 * @param g Green value
 * @param b Blue value
 */
void LEDCloudMatrix::setPixelColorByLevel(uint8_t level, uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
	struct px_level* m = &(this->map__level.map[level]);
	this->raw_strips[m->strip_id]->setPixelColor(m->offset, r, g, b);
}

/**
 * @brief Set a pixel color via the level
 * 
 * @param level The level to write to
 * @param x N pixels from start of level
 * @param c 32-bit color
 */
void LEDCloudMatrix::setPixelColorByLevel(uint8_t level, uint16_t n, uint32_t c) {
	struct px_level* m = &(this->map__level.map[level]);
	this->raw_strips[m->strip_id]->setPixelColor(m->offset, c);
}

/**
 * @brief Extend the size of the full matrix map array
 */
void LEDCloudMatrix::extend_map__full(void) {
	uint16_t old_size = this->map__full_matrix.dim;
	this->map__full_matrix.dim += 50;

	// create new map
	struct px_map** tmp_map = (struct px_map**) malloc(sizeof(struct px_map*) * this->map__full_matrix.dim);
	for (uint8_t i = 0; i < this->map__full_matrix.dim; i++) {
		tmp_map[i] = (struct px_map*) malloc(sizeof(struct px_map) * this->map__full_matrix.dim);
		for (uint8_t j = 0; j < this->map__full_matrix.dim; j++) {
			tmp_map[i][j].strip_id = -1;	// mark invalid
		}
	}

	// copy in old data
	for (uint8_t row = 0; row < this->map__full_matrix.r_y; row++) {
		for (uint8_t col = 0; col < this->map__full_matrix.r_x; col++) {
			tmp_map[row][col].strip_id = this->map__full_matrix.map[row][col].strip_id;
			tmp_map[row][col].offset = this->map__full_matrix.map[row][col].offset;
		}
	}

	// free old data
	for (uint8_t i = 0; i < old_size; i++) { free(this->map__full_matrix.map[i]); }
	free(this->map__full_matrix.map);

	// set new map as map
	this->map__full_matrix.map = tmp_map;
}

/**
 * @brief Extend the size of the top matrix map array
 */
void LEDCloudMatrix::extend_map__top(void) {
	uint16_t old_size = this->map__top_matrix.dim;
	this->map__top_matrix.dim += 20;

	// create new map
	struct px_map** tmp_map = (struct px_map**) malloc(sizeof(struct px_map*) * this->map__top_matrix.dim);
	for (uint8_t i = 0; i < this->map__top_matrix.dim; i++) {
		tmp_map[i] = (struct px_map*) malloc(sizeof(struct px_map) * this->map__top_matrix.dim);
		for (uint8_t j = 0; j < this->map__top_matrix.dim; j++) {
			tmp_map[i][j].strip_id = -1;	// mark invalid
		}
	}

	// copy in old data
	for (uint8_t row = 0; row < this->map__top_matrix.r_y; row++) {
		for (uint8_t col = 0; col < this->map__top_matrix.r_x; col++) {
			tmp_map[row][col].strip_id = this->map__top_matrix.map[row][col].strip_id;
			tmp_map[row][col].offset = this->map__top_matrix.map[row][col].offset;
		}
	}

	// free old data
	for (uint8_t i = 0; i < old_size; i++) { free(this->map__top_matrix.map[i]); }
	free(this->map__top_matrix.map);

	// set new map as map
	this->map__top_matrix.map = tmp_map;
}

/**
 * @brief Extend the size of the level map array
 */
void LEDCloudMatrix::extend_map__level(void) {
	this->map__level.dim += 5;

	// create new map
	struct px_level* tmp_map = (struct px_level*) malloc(sizeof(struct px_level) * this->map__level.dim);
	for (uint8_t i = 0; i < this->map__level.dim; i++) {
		this->map__level.map[i].strip_id = -1;	// mark invalid
	}

	// copy in old data
	for (uint8_t i = 0; i < this->map__level.r_count; i++) {
		tmp_map[i].strip_id = this->map__level.map[i].strip_id;
		tmp_map[i].offset = this->map__level.map[i].offset;
	}

	// free old data
	free(this->map__level.map);

	// set new map as map
	this->map__level.map = tmp_map;
}
