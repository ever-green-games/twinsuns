
#ifndef SPRITE_HEAD
#define SPRITE_HEAD

#define SPRITE_COUNT 32
#define SPRITE_ANIM_RATE 2.f
#define SPRITE_CHAR_FRAMES 2
#define SPRITE_MOVE_SPEED 48

#include "tilemap.h"

typedef enum {
	FACING_RIGHT,
	FACING_UP,
	FACING_DOWN,
	FACING_LEFT
} Facing;

typedef struct {
	int index; 								// index assigned to it.
	float x;
	float y;
	tile_t tile;							// sprite tiles are in 16
	int w;
	int h;
	bool follow_camera;

	// move the sprite...
	bool moving;
	float move_x;
	float move_y; 

	// whether it is a character
	bool is_char;
	tile_t base_tile;
	Facing facing;
	float frame;
} Sprite;

extern Sprite* sprites[SPRITE_COUNT];

Vector2 sprite_get_pos(Sprite* sprite);
Vector2 sprite_camera_pos(Sprite* sprite);
void add_sprite(Sprite* sprite);
void remove_sprite(Sprite* sprite);
bool sprite_off_screen(Sprite* sprite);
void check_sprite_free(Sprite* sprite);
void draw_sprite(Sprite* sprite);
void draw_sprites();
void update_sprites();

#endif
