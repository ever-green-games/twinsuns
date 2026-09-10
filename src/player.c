
#include "includes.h"

#include "command.h"
#include "render.h"
#include "map.h"
#include "window.h"
#include "event.h"
#include "message.h"
#include "player.h"

int level_table[20][6] = {
	{12, 3, 4, 3, 2, 0},
	{ 2, 1, 0, 0, 0, 225},
	{ 0, 1, 0, 0, 1, 436},
	{ 2, 0, 1, 1, 0, 873},
	{ 1, 0, 0, 1, 1, 1633}
};

Player player = {0};
Sprite player_sprite = {0};
int player_move = 0;
float player_speed = PLAYER_SPEED_NORMAL; 						// this gets multiplied by the tile being moved onto.

int get_eflag(int index)
{
	if (index >= FLAGS || index < 0)
	{
		printf("Flags exceed limit.");
		exit(1);
	}
	int chunk = floor(index / FLAG_CHUNK);
	int ii = index-chunk;
	return player.flag[chunk] & ( 1 << ii );
}

void set_eflag(int index)
{
	if (index >= FLAGS || index < 0)
	{
		printf("Flags exceed limit.");
		exit(1);
	}
	int chunk = floor(index / FLAG_CHUNK);
	int ii = index-chunk;
	player.flag[chunk] |= (1 << ii);
}

void init_player_sprite()
{
	player_sprite = (Sprite){
		.w = 1,
		.h = 1,
		.x = RENDER_W / 2 - 8,
		.y = RENDER_H / 2 - 8,
		.follow_camera=false,
		.tile = 128,
		.base_tile = 128,
		.is_char = true,
		.facing = FACING_DOWN
	};
	add_sprite(&player_sprite);
}

void init_player()
{
	init_player_sprite();
	tilebuff_scrollx = -SCROLL_OFFSET; // TODO: put this in a map init/entry thing?
	tilebuff_scrolly = -SCROLL_OFFSET;
	player_center_map();
}

void player_center_map()
{
	map_offx = player.x - 9;
	map_offy = player.y - 8;
}

/*
void try_move(Sprite* sprite, float to_x, float to_y)
{
	if (sprite->moving) return;

	int tile_x = (int)floor(to_x / TILE_W);
	int tile_y = (int)floor(to_y / TILE_H);
	int tile_index = tile_x + tile_y * TILEMAP_W;
	int tile = map.tilemap[tile_index];
	
	// set facing
	switch (sign(to_x - sprite->x))
	{
		case 0: 
			break;
		case 1: sprite->facing = FACING_RIGHT; 
			break;
		case -1: sprite->facing = FACING_LEFT; 
			break;
	}
	switch (sign(to_y - sprite->y))
	{
		case 0: 
			break;
		case -1: sprite->facing = FACING_UP;
			break;
		case 1: sprite->facing = FACING_DOWN;
			break;
	}

	// check for obstacles
	if (tile_data[tile] & TILE_SOLID) return;

	// TODO: check for events or other sprites? not sure.

	// finalize move
	sprite->moving = true;
	sprite->move_x = to_x;
	sprite->move_y = to_y;
}*/

tvec2 player_front()
{	
	char to_x = player.x;
	char to_y = player.y;

	switch (player_sprite.facing)
	{
		case FACING_UP:
			to_y-=1;
			break;
		case FACING_DOWN:
			to_y+=1;
			break;
		case FACING_LEFT:
			to_x-=1;
			break;
		case FACING_RIGHT:
			to_x+=1;
			break;
	}

	return (tvec2){ to_x, to_y };
}

bool tile_passable(int block_index)
{
	// TODO: boat and other passability permissions.
	return !(block_data[block_index][4] & TILE_SOLID || block_data[block_index][4] & TILE_WATER);
}

void player_move_forward()
{
	if (player_move > 0) return;

	tvec2 to = player_front();

	int tile = map_get_tile(to.x, to.y);
	int tflags = block_data[tile][4];
	
	// check for obstacles
	if (!tile_passable(tile)) return;
	// change speed for tile
	if (tflags & TILE_MEDIUM) 	player_speed = PLAYER_SPEED_MEDIUM; else
	if (tflags & TILE_HARD) 	player_speed = PLAYER_SPEED_SLOW; 	else
								player_speed = PLAYER_SPEED_NORMAL;	

	// finalize move
	player_move = floor(16 / player_speed);
	player.x = to.x; // move the player on the tile grid as the animation starts.
	player.y = to.y; 
}

// NOTE: will break if EVENT_SIZE becomes >256
int find_event_pos(tvec2 pos)
{
	for(int i = 0; i < EVENT_SIZE; ++i)
	{
		tvec2* event = &maps[map_index].event_pos[i];
		if (pos.x == event->x && pos.y == event->y)
		{
			return i;
		}
	}
	return -1;
}

int find_event_below()
{
	return find_event_pos((tvec2){ player.x, player.y });
}

int find_event_front()
{
	return find_event_pos(player_front());
}

void check_event_touch()
{
	int event = find_event_below();
	if (event != -1)
	{
		// TODO: is this clunky or is it just fine?
		attempt_command(event, cmd_string[CMD_STEP], "");
	}
}

void check_event_command(char* cmd, char* arg)
{
	int event = find_event_below();
	if (event == -1) event = find_event_front();
	attempt_command(event, cmd, arg);
}

void player_tilehurt(int level)
{
	// TODO: player takes damage
	// TODO: check player death
	// TODO: flash screen
}

void check_tile_touch()
{
	tile_t tile = map_get_tile(player.x, player.y);
	int tflags = block_data[tile][4];

	if (tflags & TILE_HURT1)
	{
		player_tilehurt(1);
	}
	if (tflags & TILE_HURT2)
	{
		player_tilehurt(2);
	}	
}

void update_player()
{
	// process player movement
	if (player_move != 0)
	{
		player_move -= 1;

		switch (player_sprite.facing)
		{
			case FACING_UP:
				tilebuff_scrolly+=player_speed;
				if (player_move == 0) map_offy -= 1;
				break;

			case FACING_DOWN:
				tilebuff_scrolly-=player_speed;
				if (player_move == 0) map_offy += 1;
				break;

			case FACING_LEFT:
				tilebuff_scrollx+=player_speed;
				if (player_move == 0) map_offx -= 1;
				break;

			case FACING_RIGHT:
				tilebuff_scrollx-=player_speed;
				if (player_move == 0) map_offx += 1;
				break;
		}
		
		if (player_move == 0)
		{
			// check hidden areas
			if (map_view == 0 && map_within_hidden(player.x, player.y) != -1) map_view = 1;
			if (map_view == 1 && map_within_hidden(player.x, player.y) == 0) map_view = 0;
			tilebuff_scrollx = -SCROLL_OFFSET; // TODO: const value for base scroll?
			tilebuff_scrolly = -SCROLL_OFFSET;
			map_to_tilebuff();
			// TODO: check interactions after rendering if things start competing.
			// after move effects
			check_tile_touch(); // TODO: queue up interactions if things start overlapping much
			check_event_touch();
		}
	}
	
	if (player_move == 0)
	{
		// TODO: make it try to move you in the last direction pressed.
		// TODO: for some reason there can be a segfault when moving?
		// check player movement
		if (IsKeyDown(KEY_RIGHT))
		{
			player_sprite.facing = FACING_RIGHT;
			player_move_forward();
		} else
		if (IsKeyDown(KEY_LEFT))
		{
			player_sprite.facing = FACING_LEFT;
			player_move_forward();
		} else
		if (IsKeyDown(KEY_UP))
		{
			player_sprite.facing = FACING_UP;
			player_move_forward();
		} else
		if (IsKeyDown(KEY_DOWN))
		{
			player_sprite.facing = FACING_DOWN;
			player_move_forward();
		}
		// command window
		if (IsKeyPressed(KEY_SPACE)) command_window();

		// check hotkey
		char c = GetCharPressed();
		strupper(&c);
		if (c)
		{
			char* hotkey = player.hotkey[ c-65 ];
			if (hotkey[0] != '\0')
			{
				strcpy(cmd_buffer, hotkey);
				execute_cmd_buffer();
				reset_cmd_buffer();
			}
		}

		// remove hotkey
		if (IsKeyDown(KEY_BACKSPACE))
		{
			for (c = KEY_A; c <= KEY_Z; ++c)
			{
				if (IsKeyPressed(c))
				{
					char* hotkey = player.hotkey[ c-65 ];
					if (hotkey[0] != '\0')
					{
						hotkey[0] = '\0';
						message_box("Hotkey removed.");
					}
				}
			}
		}
	}
}
