
#include "includes.h"
#include "player.h"
#include "game.h"

void init()
{
	// tilemap.h
	load_tileset();
	make_tile_rects();

	// player.h
	init_player();

	// map.h
	map_make_sprites(); // TODO: shift this to a set_map() function or the like.
	map_load(MAP_FNAME);
	map_to_tilebuff();

	// render.h
	init_render();

	/*for(int i = 0; i < TILEMAP_SIZE; ++i)
	{
		maps[map_index].tilemap[i] = 1 + rand() % 4;
	}
	maps[map_index].outer_tile = 1;
	map_to_tilebuff();*/
}

void update()
{	
	update_player();
	map_update_sprites();
	update_sprites();
	render();
}

void deinit()
{
	// TODO: implement.
}
