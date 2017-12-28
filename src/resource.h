#ifndef RESOURCE_H
#define RESOURCE_H

#include <stdint.h>

#define GLYPH_WIDTH 8
#define GLYPH_HEIGHT 8
#define TILE_WIDTH 8
#define TILE_HEIGHT 8

struct TileSet {
	uint8_t tile_count;
	uint8_t bitdepth;
	uint16_t size;
	uint8_t *data;
};

struct GlyphMap {
	uint8_t glyph_count;
	uint16_t size;
	uint8_t *data;
};

#endif
