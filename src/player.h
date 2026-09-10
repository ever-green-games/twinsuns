
#ifndef PLAYER_H
#define PLAYER_H

#include "util.h"
#include "battle.h"
#include "command.h"

// TODO: convert from sketch.
typedef struct {
	FLAG_T flag[FLAG_N];
	int x;
	int y;
	int map;
	int days;
	BattleData battler;
	char hotkey[26][CMD_BUFFER_SIZE];
} Player;

extern int level_table[20][6];

extern Player player;
extern Sprite player_sprite;
extern int player_move;
extern float player_speed;			 						// this gets multiplied by the tile being moved onto.

int get_eflag(int index);
void set_eflag(int index);

void init_player_sprite();
void init_player();
void player_center_map();

tvec2 player_front();
bool tile_passable(int block_index);
void player_move_forward();
int find_event_pos(tvec2 pos);
int find_event_below();
int find_event_front();

void check_event_touch();
void check_event_command(char* cmd, char* arg);
void player_tilehurt(int index);
void check_tile_touch();
void update_player();

#endif
