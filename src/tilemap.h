
#ifndef TILEMAP_HEAD // because TILEMAP_H in const.h
#define TILEMAP_HEAD

typedef int tile_t; // NOTE: this will be invalidated if the tileset is ever >256 tiles.

typedef struct {
	uchar x;
	uchar y;
} tvec2;

typedef struct {
	uchar x;
	uchar y;
	uchar w;
	uchar h;
} trec;

// the tilemap that will render to screen
extern tile_t tilebuff[TILEBUFF_SIZE];
extern float tilebuff_scrollx;
extern float tilebuff_scrolly;

// the tileset graphic
extern Texture2D tileset;
// cached rects for each tile
extern Rectangle tile_rects[TILESHEET_SIZE];

void load_tileset();
void make_tile_rects();
void draw_tile_buffer();
int tilebuff_index(int x, int y);
void tilebuff_set(tile_t tile, int x, int y);
tile_t tile16_to_tile8(tile_t tile);
void tilebuff_set16(tile_t tile16, int x, int y);
tile_t char_to_tile(char c);
void tilebuff_print(char* str, int x, int y);

#endif
