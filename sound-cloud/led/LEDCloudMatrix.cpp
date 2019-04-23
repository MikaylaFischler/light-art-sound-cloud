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
	this->raw_strips[strip_t::ROW_1_2] = row_1_2_strip;
	this->raw_strips[strip_t::ROW_3_4] = row_3_4_strip;
	this->raw_strips[strip_t::TOP] = top_strip;
	
	// default brightness
	for (uint8_t i = 0; i < 3; i++) {
		this->raw_strips[i]->setBrightness(150);
		this->raw_strips[i]->show();
	}

	// allocate zones
	for (uint8_t i = 0; i < 4; i++) {
		zones[i].dim = 15;
		zones[i].r_w = zones[i].r_h = 0;
		zones[i].map = (struct px_map**) malloc(sizeof(struct px_map*) * zones[i].dim);

		for (uint8_t y = 0; y < zones[i].dim; y++) {
			zones[i].map[y] = (struct px_map*) malloc(sizeof(struct px_map) * zones[i].dim);

			// mark cells invalid
			for (uint8_t x = 0; x < zones[i].dim; x++) {
				zones[i].map[y][x].strip_id = -1;
			}
		}
	}

	// allocate levels
	levels.dim = 5;
	levels.r_dim = 0;
	levels.map = (struct px_level*) malloc(sizeof(struct px_level) * levels.dim);

	for (uint8_t i = 0; i < levels.dim; i++) {
		levels.map[i].strip_id = -1;	// mark invalid
	}
}

/**
 * @brief Destroy the LEDCloudMatrix::LEDCloudMatrix object
 * @note Frees the map matricies/arrays
 */
LEDCloudMatrix::~LEDCloudMatrix() {
	// free zones
	for (uint8_t z = 0; z < 4; z++) {
		for (uint8_t i = 0; i < zones[z].dim; i++) { free(zones[z].map[i]); }
		free(zones[z].map);
	}

	// free level map
	free(levels.map);
}

/**
 * @brief Get a raw Adafruit_NeoPixel strip by ID
 * 
 * @param id Strip ID
 * @return Adafruit_NeoPixel* selected strip
 */
Adafruit_NeoPixel* LEDCloudMatrix::strip(strip_t id) const { return this->raw_strips[id]; }

/**
 * @brief Get the length of a particular level
 * 
 * @param level The level
 * @return uint16_t That level's length
 */
uint16_t LEDCloudMatrix::levelLength(uint8_t level) const { return levels.map[level].length; }

/**
 * @brief Get the width of a zone
 * 
 * @return uint16_t Width
 */
uint16_t LEDCloudMatrix::zoneWidth(zone_t zone_id) const { return zones[zone_id].r_w; }

/**
 * @brief Get the height of a zone
 * 
 * @return uint16_t Height
 */
uint16_t LEDCloudMatrix::zoneHeight(zone_t zone_id) const { return zones[zone_id].r_h; }

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
	seg->strip_id = strip;
	seg->offset = start;
	seg->length = length;
	return seg;
}

/**
 * @brief Map a segment to a zone
 * 
 * @param segment The segment to map
 * @param row The starting row
 * @param col The starting column
 * @param orientation The orientation for the segment in the matrix
 */
void LEDCloudMatrix::mapSegmentToZone(led_segment_t* segment, zone_t zone_id, uint8_t row, uint8_t col, seg_o_t orientation) {
	const uint16_t row_max = row + segment->length;
	const uint16_t row_min = row - segment->length;
	const uint16_t col_max = col + segment->length;
	const uint16_t col_min = col - segment->length;
	
	// map the segment
	switch (orientation) {
		case COLUMN_POS:
			// make sure we can fit this and update the real lengths if necessary
			while (row_max >= zones[zone_id].dim || col >= zones[zone_id].dim) { extend_zone(zone_id); }
			if (zones[zone_id].r_w < col) { zones[zone_id].r_w = col; }
			if (zones[zone_id].r_h < row_max) { zones[zone_id].r_h = row_max; }

			for (uint16_t r = 0; r < segment->length; r++) {
				zones[zone_id].map[row + r][col].strip_id = segment->strip_id;
				zones[zone_id].map[row + r][col].offset = segment->offset + r;
			}
		break;
		case COLUMN_NEG:
			// make sure we can fit this and update the real lengths if necessary
			while (row >= zones[zone_id].dim || col >= zones[zone_id].dim) { extend_zone(zone_id); }
			if (zones[zone_id].r_w < col) { zones[zone_id].r_w = col; }
			if (zones[zone_id].r_h < row) { zones[zone_id].r_h = row; }

			for (uint16_t r = 0; r < segment->length && (row - r) >= 0; r++) {
				zones[zone_id].map[row - r][col].strip_id = segment->strip_id;
				zones[zone_id].map[row - r][col].offset = segment->offset + r;
			}
		break;
		case ROW_POS:
			// make sure we can fit this and update the real lengths if necessary
			while (row >= zones[zone_id].dim || col_max >= zones[zone_id].dim) { extend_zone(zone_id); }
			if (zones[zone_id].r_w < col_max) { zones[zone_id].r_w = col_max; }
			if (zones[zone_id].r_h < row) { zones[zone_id].r_h = row; }

			for (uint16_t c = 0; c < segment->length; c++) {
				zones[zone_id].map[row][col + c].strip_id = segment->strip_id;
				zones[zone_id].map[row][col + c].offset = segment->offset + c;
			}
		break;
		case ROW_NEG:
			// make sure we can fit this and update the real lengths if necessary
			while (row >= zones[zone_id].dim || col >= zones[zone_id].dim) { extend_zone(zone_id); }
			if (zones[zone_id].r_w < col) { zones[zone_id].r_w = col; }
			if (zones[zone_id].r_h < row) { zones[zone_id].r_h = row; }

			for (uint16_t c = 0; c < segment->length && (col - c); c++) {
				zones[zone_id].map[row][col - c].strip_id = segment->strip_id;
				zones[zone_id].map[row][col - c].offset = segment->offset + c;
			}
		break;
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
	while (level >= levels.dim) { extend_level(); }
	if (levels.r_dim < level + 1) { levels.r_dim = level + 1; }

	// set the map
	levels.map[level].strip_id = segment->strip_id;
	levels.map[level].offset = segment->offset;
}

/**
 * @brief Set a pixel color via zones
 * 
 * @param zone_id The zone's ID
 * @param x X coordinate (column)
 * @param y Y coordinate (row)
 * @param r Red value
 * @param g Green value
 * @param b Blue value
 */
void LEDCloudMatrix::setPixelColorByZone(zone_t zone_id, uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
	struct px_map* m = &(zones[zone_id].map[x][y]);
	this->raw_strips[m->strip_id]->setPixelColor(m->offset, r, g, b);
}

/**
 * @brief Set a pixel color via zones
 * 
 * @param zone_id The zone's ID
 * @param x X coordinate (column)
 * @param y Y coordinate (row)
 * @param c 32-bit color
 */
void LEDCloudMatrix::setPixelColorByZone(zone_t zone_id, uint8_t x, uint8_t y, uint32_t c) {
	struct px_map* m = &(zones[zone_id].map[x][y]);
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
	struct px_level* m = &(levels.map[level]);
	this->raw_strips[m->strip_id]->setPixelColor(m->offset + n, r, g, b);
}

/**
 * @brief Set a pixel color via the level
 * 
 * @param level The level to write to
 * @param x N pixels from start of level
 * @param c 32-bit color
 */
void LEDCloudMatrix::setPixelColorByLevel(uint8_t level, uint16_t n, uint32_t c) {
	struct px_level* m = &(this->levels.map[level]);
	this->raw_strips[m->strip_id]->setPixelColor(m->offset + n, c);
}

/**
 * @brief Show all LED strips (write the changes)
 * 
 */
void LEDCloudMatrix::show(void) {
	for (uint8_t i = 0; i < 3; i++) { raw_strips[i]->show(); }
}

/**
 * @brief Show the LED strip corresponding to the level (write the changes)
 * 
 * @param level The level to update
 */
void LEDCloudMatrix::showLevel(uint8_t level) {
	if (level == 0 || level == 1) {
		raw_strips[strip_t::ROW_1_2]->show();
	} else if (level == 2 || level == 3) {
		raw_strips[strip_t::ROW_3_4]->show();
	}
}

/**
 * @brief Show LED strip(s) by zone (write the changes)
 * 
 */
void LEDCloudMatrix::showZone(zone_t zone_id) {
	switch (zone_id) {
		case zone_t::RIGHT:
		case zone_t::FRONT:
		case zone_t::LEFT:
			raw_strips[strip_t::ROW_1_2]->show();
			raw_strips[strip_t::ROW_3_4]->show();
			break;
		case zone_t::TOP:
			raw_strips[strip_t::ROW_3_4]->show();
			raw_strips[strip_t::TOP]->show();
			break;
	}
}

/**
 * @brief Extend the size of a zone
 * 
 * @param zone_id The zone's identifier
 */
void LEDCloudMatrix::extend_zone(zone_t zone_id) {
	uint16_t old_size = zones[zone_id].dim;
	zones[zone_id].dim += 15;

	// create new map
	struct px_map** tmp_map = (struct px_map**) malloc(sizeof(struct px_map*) * zones[zone_id].dim);
	for (uint8_t i = 0; i < zones[zone_id].dim; i++) {
		tmp_map[i] = (struct px_map*) malloc(sizeof(struct px_map) * zones[zone_id].dim);
		for (uint8_t j = 0; j < zones[zone_id].dim; j++) {
			tmp_map[i][j].strip_id = -1;	// mark invalid
		}
	}

	// copy in old data
	for (uint8_t row = 0; row < zones[zone_id].r_h; row++) {
		for (uint8_t col = 0; col < zones[zone_id].r_w; col++) {
			tmp_map[row][col].strip_id = zones[zone_id].map[row][col].strip_id;
			tmp_map[row][col].offset = zones[zone_id].map[row][col].offset;
		}
	}

	// free old data
	for (uint8_t i = 0; i < old_size; i++) { free(zones[zone_id].map[i]); }
	free(zones[zone_id].map);

	// set new map as map
	zones[zone_id].map = tmp_map;
}

/**
 * @brief Extend the size of the level map array
 */
void LEDCloudMatrix::extend_level(void) {
	levels.dim += 5;

	// create new map
	struct px_level* tmp_map = (struct px_level*) malloc(sizeof(struct px_level) * levels.dim);
	for (uint8_t i = 0; i < levels.dim; i++) {
		levels.map[i].strip_id = -1;	// mark invalid
	}

	// copy in old data
	for (uint8_t i = 0; i < levels.r_dim; i++) {
		tmp_map[i].strip_id = levels.map[i].strip_id;
		tmp_map[i].offset = levels.map[i].offset;
		tmp_map[i].length = levels.map[i].length;
	}

	// free old data
	free(levels.map);

	// set new map as map
	levels.map = tmp_map;
}
