
#include "includes.h"
#include "tilemap.h"

// the tilemap that will render to screen
tile_t tilebuff[TILEBUFF_SIZE] = {0};
float tilebuff_scrollx = 0;
float tilebuff_scrolly = 0;

// the tileset graphic
Texture2D tileset;
// cached rects for each tile
Rectangle tile_rects[TILESHEET_SIZE] = {0};

void load_tileset()
{
	if (IsTextureValid(tileset)) UnloadTexture(tileset); // for repeated calls.
	tileset = LoadTexture(TILESHEET_FNAME);
	if (!IsTextureValid(tileset))
	{
		printf("Failed to load tileset.");
		exit(1);
	}
}

void make_tile_rects()
{
	for(int i = 0; i < TILESHEET_SIZE; ++i)
	{
		float x = (i % TILESHEET_W) * TILE_W;
		float y = (floor(i / TILESHEET_W)) * TILE_H;

		tile_rects[i] = (Rectangle){
			x, y,
			TILE_W,
			TILE_H
		};
	}
}

void draw_tile_buffer()
{
	Vector2 pos = {0, 0};
	int offx = floor(tilebuff_scrollx);
	int offy = floor(tilebuff_scrolly);

	for(int i = 0; i < TILEBUFF_SIZE; ++i)
	{
		tile_t tile = tilebuff[i];
		pos.x = offx + (i % TILEBUFF_W) * TILE_W;
		pos.y = offy + (floor(i / TILEBUFF_H)) * TILE_H;

		DrawTextureRec(
			tileset,
			tile_rects[tile],
			pos,
			WHITE
		);
	}
}

int tilebuff_index(int x, int y)
{
	if (x < 0 || y < 0 || x >= TILEBUFF_W || y >= TILEBUFF_H)
	{
		printf("Invalid tilebuff index.\n");
		exit(1);
		//return 0;
	}
	return x + y * TILEBUFF_W;
}

void tilebuff_set(tile_t tile, int x, int y)
{
	tilebuff[tilebuff_index(x, y)] = tile;	
}

tile_t tile16_to_tile8(tile_t tile)
{
	// TODO: is there a simpler way to compute this? I'm nearly converting into x/y and then back into index...
	tile_t row = tile / TILESHEET_W16;
	return (row * 2 * TILESHEET_W) + (tile - TILESHEET_W16*row) * 2;
}

// uses 16 tile and x/y coordinates.
void tilebuff_set16(tile_t tile16, int x, int y)
{
	int index = tilebuff_index(x*2, y*2);
	int tile8 = tile16_to_tile8(tile16);
	tilebuff[index] 				= tile8;
	tilebuff[index+1] 				= tile8+1;
	tilebuff[index+TILEBUFF_W] 		= tile8+TILESHEET_W;
	tilebuff[index+TILEBUFF_W+1] 	= tile8+TILESHEET_W+1;
}

tile_t char_to_tile(char c)
{
	// convert ranges
	// A-Z
	if (c >= 65 && c <= 90) return 938 + (c - 65); // tile index minus char, offset by start of range.
	// a-z
	if (c >= 97 && c <= 122) return 964 + (c - 97);
	// 0-9
	if (c >= 48 && c <= 57) return 928 + (c - 48);
	
	// convert individual characters
	switch (c)
	{
		case'\'': return 990;
		case ',': return 991;
		case '.': return 992;
		case ' ': return 993;
		case '-': return 994;
		case '!': return 996;
		case '?': return 997;
	}

	return 132; // return a tree so it's obvious something is going wrong.
}

void tilebuff_print(char* str, int x, int y)
{
	for(int i = 0; i < strlen(str); ++i)
	{
		tilebuff_set(
			char_to_tile(str[i]),
			x + i,
			y
		);
	}
}
