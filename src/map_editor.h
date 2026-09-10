
#ifndef MAP_EDITOR_HEAD
#define MAP_EDITOR_HEAD

#include <stdbool.h>

typedef struct {
	int x;
	int y;
} vec2i;

typedef struct {
	bool active;
	vec2i cursor;
	vec2i cursor_dragstart;
	vec2i cursor_tile;
	vec2i cursor_event;
	uchar tile;
	char input_mode;
	char hidden_index;
} MapEditor;

extern MapEditor map_editor;

void map_editor_check_hidden_mode(); 
void map_editor_input_mode(char input_mode);
void update_map_editor();
void medit_check_cursor_pan(vec2i moved);
void update_map_editor_cursor();
void update_map_editor_tile();
void update_map_editor_event();
void update_map_editor_hidden();
void start_map_editor();
void medit_draw_cursor(vec2i* cursor);
vec2i medit_update_cursor(vec2i* cursor);
void map_save_file(char* fname);
void map_save();
void map_clear();
void medit_player_move();
void medit_draw_event(int event_index, int x, int y);
void medit_draw_event_data(int event_index);

#endif
