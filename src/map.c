
#include "includes.h"
#include "event.h"

Map maps[MAP_SIZE];
int map_index = 0;
int map_offx = 0;
int map_offy = 0;
uchar map_view = 0;
Sprite map_sprites[EVENT_SIZE] = {0};

bool map_within_bounds(int x, int y)
{
	return !(x < 0 || y < 0 || x >= TILEMAP_W || y >= TILEMAP_H); 
}

int map_within_hidden(int x, int y)
{
	for (int i = 0; i < MAP_HIDDEN_CT; ++i)
	{
		trec rec = maps[map_index].hidden[i];
		if (maps[map_index].hidden_tile[i] == 0) continue;
		if (map_view == 0)
		{

		}
		if (x >= rec.x && y >= rec.y && x <= rec.x+rec.w && y <= rec.y+rec.h)
		{
			if (map_view == 0) return maps[map_index].hidden_tile[i];
			else return -1;
		}
		else
		{
			if (map_view == 1) return 0;
		}
	}
	return -1;
}

tile_t map_get_tile(int x, int y)
{
	if (!map_within_bounds(x, y)) return maps[map_index].outer_tile;
	return maps[map_index].tilemap[x + y * TILEMAP_W];
}

Vector2 map_get_scroll()
{
	return (Vector2){ 
		floor((float)(map_offx * TILE_W16) - tilebuff_scrollx),
		floor((float)(map_offy * TILE_H16) - tilebuff_scrolly)
	};
}

void block_to_tilebuff(int tile, int x, int y)
{
	tilebuff_set(block_data[tile][0], x,   y  );
	tilebuff_set(block_data[tile][1], x+1, y  );
	tilebuff_set(block_data[tile][2], x,   y+1);
	tilebuff_set(block_data[tile][3], x+1, y+1);
}

// x,y = the location on the tile buffer.
// use case: refreshing parts of the map on the tilebuffer selectively. (window animations.)
void map_to_tilebuff_tile(int x, int y)
{
	//tilebuff_set16(map_get_tile(x+map_offx, y+map_offy), x, y);
	int tile = map_get_tile(x+map_offx, y+map_offy);
	int r = map_within_hidden(x+map_offx, y+map_offy);
	if (r != -1) tile = r;
	block_to_tilebuff(tile, x*2, y*2);
}

// put the visible portion of the map into the tile buffer.
void map_to_tilebuff()
{
	for(int y = 0; y < TILEBUFF_H16; ++y)
	{
		for (int x = 0; x < TILEBUFF_W16; ++x)
		{
			map_to_tilebuff_tile(x, y);
		}
	}
}

void map_make_sprites()
{
	for( int i = 0; i < EVENT_SIZE; ++i)
	{
		Event* event = &events[map_index][i];
		tvec2* event_pos = &maps[map_index].event_pos[i];
		if (event->tile != 0)
		{
			map_sprites[i] = (Sprite){
				.x = event_pos->x * TILE_W16,
				.y = event_pos->y * TILE_H16,
				.follow_camera = true,
				.tile = event->tile,
				.base_tile = event->tile,
				.w = event->eflags & EVENT_LARGE ? 2 : 1,
				.h = event->eflags & EVENT_LARGE ? 2 : 1,
				.is_char = event->eflags & EVENT_ISCHAR
			};
		}
	}
}

void map_update_sprites()
{	
	for( int i = 0; i < EVENT_SIZE; ++i)
	{
		Event* event = &events[map_index][i];
		Sprite* sprite = &map_sprites[i];
		// check if this event even cares about sprites
		if (event->tile != 0)
		{
			if (sprite->index != -1)
			{
				if (event->eflags & EVENT_WANDERS)
				{
					// TODO: make event wander...
				}
			}
			else
			{
				// check if event is on-screen and try to make it active.
				if (!sprite_off_screen(sprite)) add_sprite( sprite );
			}
		}
	}
}

void map_load(char* fname)
{
	FILE* f = fopen(MAP_FNAME, "rb");
	fread(&maps, sizeof(maps), 1, f);
	fclose(f);
}

#define TILE4(x) x, x+1, x+32, x+33

tile_t block_data[MAP_BLOCK_CT][5] = {
	[0]   = {0},													// blank
	[1]   = { 1, 2, 2, 1},											// grass
	[2]   = { 3, 3, 3, 3, 			TILE_MEDIUM | TILE_ENC1}, 		// forest
	[3]   = { TILE4( 4 ), 			TILE_HARD | TILE_ENC2 }, 		// hill
	[4]   = { TILE4( 6 ), 			TILE_SOLID },					// mountain
	[5]   = { 34, 34, 34, 34, 		TILE_HARD | TILE_ENC1},			// desert
	[6]   = { 35, 35, 35, 35, 		TILE_MEDIUM},					// palm tree
	[7]   = { 98, 66, 98, 66, 		TILE_ENC2}, 					// dark grass
	[8]   = { 67, 67, 67, 67, 		TILE_MEDIUM | TILE_ENC2}, 		// dark forest
	[9]   = { TILE4( 70 ), 			TILE_HARD | TILE_ENC2 }, 		// trash
	[16]  = { 8, 9, 40, 41, 		TILE_WATER}, 					// top-left water
	[17]  = { 9, 9, 41, 41, 		TILE_WATER}, 					// top water
	[18]  = { 9, 10, 41, 42, 		TILE_WATER}, 					// top-right water
	[32]  = { 40, 41, 40, 41,		TILE_WATER}, 					// left water
	[33]  = { 41, 11, 11, 41, 		TILE_WATER},			 		// center water
	[34]  = { 41, 42, 41, 42, 		TILE_WATER}, 					// right water
	[48]  = { 40, 41, 72, 73, 		TILE_WATER}, 					// bottom-left water
	[49]  = { 41, 41, 73, 73, 		TILE_WATER}, 					// bottom water
	[50]  = { 41, 42, 73, 74,		TILE_WATER}, 					// bottom-right water
	[19]  = { 9, 9, 73, 73, 		TILE_WATER},					// horizontal water
	[51]  = { 40, 42, 40, 42, 		TILE_WATER},					// vertical water
	[20]  = { 8, 10, 40, 42, 		TILE_WATER}, 					// top cap water
	[35]  = { 8, 9, 72, 73, 		TILE_WATER}, 					// left cap water
	[37]  = { 9, 10, 73, 74, 		TILE_WATER},					// right cap water
	[52]  = { 40, 42, 72, 74, 		TILE_WATER}, 					// bottom cap water
	[36]  = { 12, 13, 13, 12, 		TILE_WATER | TILE_ENC2}, 		// dark water
	[21]  = { TILE4( 14 ), 			TILE_WATER }, 					// whirlpool
	[53]  = { TILE4( 68 ) }, 										// bridge
	[38]  = { 43, 43, 43, 43, 		TILE_SOLID }, 					// cyber wall top
	[54]  = { 43, 43, 75, 75, 		TILE_SOLID },					// cyber wall
	[55]  = { TILE4( 44 ), 			TILE_SOLID },					// cyber door
	[10]  = { TILE4( 64 ) },										// landing strip
	[22]  = { TILE4( 128 ) }, 										// building
	{ TILE4( 130 ) }, 												// village
	{ TILE4( 132 ) }, 												// cyber city 1
	{ TILE4( 134 ) },												// cyber city 2
	{ TILE4( 136 ) }, 												// shrine
	{ TILE4( 138 ) }, 												// factory
	[39]  = { 107, 107, 107, 107},									// tiled floor 1
	{ TILE4( 108 ),					TILE_SOLID },					// book case
	{ 104, 105, 104, 105, 			TILE_SOLID }, 					// solar panels
	{ 106, 106, 106, 106},											// tiled floor 2
	{ 33, 33, 32, 32, 				TILE_SOLID },					// concrete wall bottom
	{ TILE4( 18 ), 					TILE_SOLID },					// concrete wall top
	{ 99, 99, 99, 99 }, 											// dirt road
	{ TILE4( 16 ), 					TILE_SOLID }, 					// big tree
};
