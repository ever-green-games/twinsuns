#include "includes.h"
#include "event.h"
#include "player.h"
#include "window.h"
#include "map_editor.h"

#define MAP_HISTORY_FORMAT "map_history/map_%ld"

#define MODE_PUT_TILE 0
#define MODE_PICK_TILE 1
#define MODE_PICK_EVENT 2
#define MODE_PUT_HIDDEN 3

#define SCREEN_W (int)ceil(RENDER_W/16)
#define SCREEN_H (int)ceil(RENDER_H/16)
#define SCROLL_MARGIN 2

MapEditor map_editor = {0};

bool __map_editor_update = false;
int hidden_key = 0;

void update_map_editor()
{
	if (!map_editor.active) return;
	if (__map_editor_update) return; // avoid recursion.

	__map_editor_update = true;

	Camera2D camera = {
		.target = map_get_scroll(),
		.zoom = 1.f
	};
	
	switch ( map_editor.input_mode )
	{
		case MODE_PUT_TILE:
			
			BeginMode2D(camera);
				for (int i = 0; i < EVENT_SIZE; ++i)
				{
					tvec2* event_pos = &maps[map_index].event_pos[i];
					int x = event_pos->x*TILE_W16;
					int y = event_pos->y*TILE_H16;
					if (x != 0 || y != 0)
					{
						DrawRectangleLines(x, y, TILE_W16, TILE_H16, YELLOW);	
						medit_draw_event(i, x, y);
					}
				}
				medit_draw_cursor(&map_editor.cursor);
			EndMode2D();

			update_map_editor_cursor();
			
			// print cursor pos
			char buff[128] = {0};
			sprintf(buff, " %d, %d ", map_editor.cursor.x, map_editor.cursor.y);
			tilebuff_print(buff, 3, 3);
			
			if (IsKeyPressed(KEY_T)) map_editor_input_mode(MODE_PICK_TILE);
			if (IsKeyPressed(KEY_E)) map_editor_input_mode(MODE_PICK_EVENT);
			map_editor_check_hidden_mode();

			break;

		case MODE_PICK_TILE:
			medit_draw_cursor(&map_editor.cursor_tile);
			update_map_editor_tile();
			if (IsKeyPressed(KEY_T) || IsKeyPressed(KEY_B)) map_editor_input_mode(MODE_PUT_TILE);
			
			break;

		case MODE_PICK_EVENT:
			DrawRectangle(0, 0, RENDER_W, RENDER_H/2+TILE_H16, BLACK);
			for (int i = 0; i < EVENT_SIZE; ++i)
			{
				int x = i % SCREEN_W;
				int y = i / SCREEN_W;
				medit_draw_event(i, x*TILE_W16, y*TILE_H16);
			}
			medit_draw_cursor(&map_editor.cursor_event);	

			update_map_editor_event();

			break;

		case MODE_PUT_HIDDEN:
			BeginMode2D(camera);
				update_map_editor_hidden();
			EndMode2D();
			if (IsKeyReleased(hidden_key)) map_editor_input_mode(MODE_PUT_TILE);
			break;
	}

	if (IsControlDown() && IsKeyPressed(KEY_S)) map_save();
	if (IsControlDown() && IsKeyPressed(KEY_N)) map_clear();
	if (IsControlDown() && IsKeyPressed(KEY_R)) load_tileset();
	if (IsControlDown() && IsKeyPressed(KEY_P)) medit_player_move();

	__map_editor_update = false;
}

void medit_draw_cursor(vec2i* cursor)
{
	DrawRectangleLines(
		cursor->x * TILE_W16,
		cursor->y * TILE_H16,
		TILE_W16,
		TILE_H16,
		WHITE
	);
}

vec2i medit_update_cursor(vec2i* cursor)
{
	vec2i moved = { 0, 0};
	
	if (IsKeyPressed(KEY_UP) 	|| IsKeyPressedRepeat(KEY_UP)	)	moved.y = -1; 
	if (IsKeyPressed(KEY_LEFT) 	|| IsKeyPressedRepeat(KEY_LEFT)	) 	moved.x = -1;
	if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) 	moved.x =  1;
	if (IsKeyPressed(KEY_DOWN) 	|| IsKeyPressedRepeat(KEY_DOWN)	) 	moved.y =  1;

	cursor->x += moved.x;
	cursor->y += moved.y;

	return moved;
}

void map_editor_check_hidden_mode()
{
	for (int i = 0; i < MAP_HIDDEN_CT; ++i)
	{
		if (IsKeyPressed(KEY_ONE+i))
		{
			hidden_key = KEY_ONE+i;
			map_editor.hidden_index = i;
			map_editor_input_mode(MODE_PUT_HIDDEN);

			return;
		}
	}
}

void map_editor_input_mode(char input_mode)
{
	char from = map_editor.input_mode;
	map_editor.input_mode = input_mode;

	switch (from)
	{
		// when exiting tile mode, refresh the tile buffer
		case MODE_PICK_TILE:
			
			map_to_tilebuff();
			break;
	}
	switch (input_mode)
	{
		// when entering tile mode, set the tile buffer to each block.
		case MODE_PICK_TILE:
			
			for(int i = 0; i < MAP_BLOCK_CT; ++i)
			{
				int x = i % 16;
				int y = i / 16;
				block_to_tilebuff(i, 3+x*2, 3+y*2);
			}
			break;
		
		case MODE_PUT_HIDDEN:

			map_editor.cursor_dragstart = map_editor.cursor;
			break;
	}
}

void medit_check_cursor_pan(vec2i moved)
{
	if (moved.y == -1) 
	{
		if (map_editor.cursor.y < map_offy+SCROLL_MARGIN+2)
		{
			map_offy -= 1;
			map_to_tilebuff();
		}
	}
	if (moved.x == -1) 
	{
		if (map_editor.cursor.x < map_offx+SCROLL_MARGIN+2)
		{
			map_offx -= 1;
			map_to_tilebuff();
		}
	}
	if (moved.x == 1)
	{
		if (map_editor.cursor.x > map_offx+SCREEN_W-SCROLL_MARGIN)
		{
			map_offx += 1;
			map_to_tilebuff();
		}
	}
	if (moved.y == 1) 
	{
		if (map_editor.cursor.y > map_offy+SCREEN_H-SCROLL_MARGIN)
		{
			map_offy += 1;
			map_to_tilebuff();
		}
	}
}

void update_map_editor_cursor()
{
	vec2i moved = medit_update_cursor(&map_editor.cursor);
	medit_check_cursor_pan(moved);

	// check event below
	if (moved.x != 0 || moved.y != 0)
	{
		int event_index = find_event_pos((tvec2){ map_editor.cursor.x, map_editor.cursor.y });
		if (event_index != -1)
			medit_draw_event_data(event_index);
		else
			map_to_tilebuff();
	} 

	// place tile
	if (IsKeyPressed(KEY_SPACE) || (IsKeyDown(KEY_SPACE) && (moved.x || moved.y)))
	{
		if (map_within_bounds(map_editor.cursor.x, map_editor.cursor.y))
			maps[map_index].tilemap[map_editor.cursor.x + map_editor.cursor.y * TILEMAP_W] = map_editor.tile;
		else
			maps[map_index].outer_tile = map_editor.tile;
		map_to_tilebuff();
	}

	// remove event
	if (IsKeyPressed(KEY_D))
	{
		int event_index = find_event_pos((tvec2){ map_editor.cursor.x, map_editor.cursor.y });
		if (event_index != -1) maps[map_index].event_pos[event_index] = (tvec2){0, 0};
		map_to_tilebuff();
	}

	// hidden view
	if (IsKeyPressed(KEY_H))
	{
		map_view = !map_view;
		map_to_tilebuff();
	}
}

void draw_map_editor_tile()
{
	
}

void update_map_editor_tile()
{
	vec2i moved = medit_update_cursor(&map_editor.cursor_tile);

	if (moved.x != 0 || moved.y != 0)
	{
		map_editor.tile = map_editor.cursor_tile.x + map_editor.cursor_tile.y * TILESHEET_W16;	
	}
}

void update_map_editor_event()
{
	vec2i moved = medit_update_cursor(&map_editor.cursor_event);

	int event_index = map_editor.cursor_event.x + map_editor.cursor_event.y * SCREEN_W;

	if (IsKeyPressed(KEY_E)) map_editor_input_mode(MODE_PUT_TILE);

	if (event_index < 0 || event_index >= EVENT_SIZE) return;
	
	// print event data
	//if (moved.x != 0 || moved.y != 0)
	{
		map_to_tilebuff();
		tvec2* event_pos = &maps[map_index].event_pos[event_index];
		char buff[32];
		sprintf(buff, "%d, %d", event_pos->x, event_pos->y);
		tilebuff_print(buff, 3, 1+SCREEN_H*2);
		medit_draw_event_data(event_index);
	}

	if (IsKeyPressed(KEY_SPACE))
	{
		maps[map_index].event_pos[event_index].x = map_editor.cursor.x;
		maps[map_index].event_pos[event_index].y = map_editor.cursor.y;
		map_editor.input_mode = MODE_PUT_TILE;
	}
}

void update_map_editor_hidden()
{
	vec2i moved = medit_update_cursor(&map_editor.cursor);
	medit_check_cursor_pan(moved);

	uchar x = fmin(map_editor.cursor.x, map_editor.cursor_dragstart.x);
	uchar y = fmin(map_editor.cursor.y, map_editor.cursor_dragstart.y);
	uchar w = fmax(map_editor.cursor.x, map_editor.cursor_dragstart.x) - x;
	uchar h = fmax(map_editor.cursor.y, map_editor.cursor_dragstart.y) - y;
	
	DrawRectangleLines(
		x * TILE_W16,
		y * TILE_H16,
		w * TILE_W16,
		h * TILE_H16,
		BLUE
	);

	if (IsKeyReleased(hidden_key))
	{
		// if you didn't move, clear the hidden zone
		if (map_editor.cursor.x == map_editor.cursor_dragstart.x &&
			map_editor.cursor.y == map_editor.cursor_dragstart.y)
		{
			maps[map_index].hidden[map_editor.hidden_index] = (trec){ 0 };
			maps[map_index].hidden_tile[map_editor.hidden_index] = 0;
		}
		// otherwise, set it!
		else
		{
			maps[map_index].hidden[map_editor.hidden_index] = (trec){ x, y, w-1, h-1 };
			maps[map_index].hidden_tile[map_editor.hidden_index] = map_editor.tile;
		}
		map_editor_input_mode(MODE_PUT_TILE);
		map_to_tilebuff();
	}
}

void start_map_editor()
{
	printf("Starting map editor.\n");

	map_editor.active = true;
	map_editor.cursor.x = player.x;
	map_editor.cursor.y = player.y;

	draw_window_wipe(COMMAND_WINDOW);
	remove_sprite(&player_sprite);	

	while(map_editor.active)
	{
		if (IsKeyPressed(KEY_ESCAPE))
		{
			map_editor.active = false;
		}
		render();
	}

	init_player_sprite();
	player_center_map();
	map_to_tilebuff();
	render();
}

void map_save_file(char* fname)
{
	FILE* f = fopen(fname, "wb");
	fwrite(&maps, sizeof(maps), 1, f);
	fclose(f);
}

void map_save()
{
	map_save_file(MAP_FNAME);

	// TODO: make this work?
	// it isn't because of recursive update_map_editor() calls probably.

	tilebuff_print("Save backup? y.n", 3, 3);
	render();
	while (true)
	{
		if (IsKeyPressed(KEY_Y))
		{
			char fname[1024] = {0};
			sprintf(fname, MAP_HISTORY_FORMAT, time(NULL));
			map_save_file(fname);
			break;
		}
		if (IsKeyPressed(KEY_N)) break;
		render();
	}

	//char fname[1024] = {0};
	//sprintf(fname, MAP_HISTORY_FORMAT, time(NULL));
	//map_save_file(fname);

	printf("Saved map data.\n");
	tilebuff_print("Saved map data.  ", 3, 3);
}

void map_clear()
{
	tilebuff_print("Clear map with current tile? y.n", 3, 3);
	render();
	while(true)
	{
		if (IsKeyPressed(KEY_Y))
		{
			for ( int i = 0; i < TILEMAP_SIZE; ++i)
			{
				maps[map_index].tilemap[i] = map_editor.tile;
			}
			break;
		}
		if (IsKeyPressed(KEY_N)) break;
		render();
	}
	map_to_tilebuff();
}

void medit_player_move()
{
	player.x = map_editor.cursor.x;
	player.y = map_editor.cursor.y;
	map_editor.active = false;
}

void medit_draw_event(int event_index, int x, int y)
{
	Event* event = &events[map_index][event_index];
	//tvec2* pos = &maps[map_index].event_pos[event_index];

	// draw a rectangular frame.


	if (event->tile != 0)
	{
		// draw the tile for it.
		tile_t tile_index = tile16_to_tile8(event->tile);

		Rectangle tile_rect = tile_rects[tile_index];
		tile_rect.width *= 2;
		tile_rect.height *= 2;

		DrawTextureRec(
			tileset,
			tile_rect,
			(Vector2){ x, y },
			WHITE
		);
	}
}

void medit_draw_event_data(int event_index)
{
	char buff[32] = {0};
	sprintf(buff, "%d %s", event_index, events[map_index][event_index].name);
	tilebuff_print(buff, 3, 2+SCREEN_H*2);
}
