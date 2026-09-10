
#ifndef MAP_HEAD
#define MAP_HEAD

#define MAP_HIDDEN_CT 4

typedef struct {
	uchar tilemap[TILEMAP_SIZE]; 		// this can still be addressed in 'char' even though tile_t is int, unless tilesheets get bigger.
	uchar encounter_id[TILEMAP_SIZE];
	tvec2 event_pos[EVENT_SIZE];
	trec hidden[MAP_HIDDEN_CT];			// bounds for hidden spaces in the map. (up to 4.) TODO: implement.
	uchar hidden_tile[MAP_HIDDEN_CT];
	uchar exit_map;						// index of map to move to when walking outside the current map. TODO: implement.
	uchar outer_tile; 					// tile to draw outside the map bounds.
} Map;

extern Map maps[MAP_SIZE];
extern int map_index;
extern int map_offx;
extern int map_offy;
extern uchar map_view;					// 0 = map, 1 = hidden area
extern Sprite map_sprites[EVENT_SIZE];

Vector2 map_get_scroll();
int map_within_hidden(int x, int y);
bool map_within_bounds(int x, int y);
tile_t map_get_tile(int x, int y);
void block_to_tilebuff(int block, int x, int y);
void map_to_tilebuff_tile(int x, int y);
void map_to_tilebuff();
void map_make_sprites();
void map_update_sprites();
void map_load(char* fname);

enum TileFlags {
	TILE_SOLID 		= 1 << 0,
	TILE_COUNTER 	= 1 << 1,					// when interacting at one of these, will check the next tile forward for an event.
	TILE_HURT1 		= 1 << 2,
	TILE_HURT2 		= 1 << 3,
	TILE_MEDIUM 	= 1 << 4,					// affects move speed
	TILE_HARD 		= 1 << 5,
	TILE_WATER 		= 1 << 6,					// only boat moves through water?
	TILE_ENC1		= 1 << 7,					// increment encounter rate by 1 more
	TILE_ENC2		= 1 << 8					// increment encounter rate by 2 more (compatible with previous flag)
};

#define MAP_BLOCK_CT 256

extern tile_t block_data[MAP_BLOCK_CT][5];

#endif
