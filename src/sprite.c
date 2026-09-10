
#include "includes.h"

Sprite* sprites[SPRITE_COUNT] = {0};

void draw_sprite(Sprite* sprite)
{
	// tile 0 is blank/nothing always.
	if (sprite->tile==0) return;

	tile_t tile = sprite->tile;
	bool flip = false;
	if (sprite->is_char)
	{
		// mod 3 = loops back to the first sprite for FACING_LEFT, so we can flip it.
		tile = sprite->base_tile + (sprite->facing % 3) * SPRITE_CHAR_FRAMES + (int)floor(sprite->frame) % SPRITE_CHAR_FRAMES;
		if (sprite->facing == FACING_LEFT) flip = true;
	}

	Rectangle draw_rect = tile_rects[tile16_to_tile8(tile)];
	draw_rect.width *= sprite->w*2;
	draw_rect.height *= sprite->h*2;
	if (flip)
	{
		draw_rect.width *= -1;
	}


	DrawTextureRec(
		tileset,
		draw_rect,
		sprite_get_pos(sprite),
		WHITE
	);
}

Vector2 sprite_get_pos(Sprite* sprite)
{
	return sprite->follow_camera ? sprite_camera_pos(sprite) : (Vector2){ sprite->x, sprite->y };
}

Vector2 sprite_camera_pos(Sprite* sprite)
{
	return (Vector2){
		sprite->x + floor(tilebuff_scrollx) - map_offx * TILE_W16,
		sprite->y + floor(tilebuff_scrolly) - map_offy * TILE_H16
	};
}

void add_sprite(Sprite* sprite)
{
	for(int i = 0; i < SPRITE_COUNT; ++i)
	{
		if (sprites[i] == NULL) 
		{
			sprites[i] = sprite;
			sprite->index = i;
			return;
		}
	}
	printf("Sprite limit exceeded. Do I need to re-evaluate something?");
	exit(1);
}

bool sprite_off_screen(Sprite* sprite)
{
	Vector2 pos = sprite_get_pos(sprite);
	return (pos.x < -TILE_W16 * sprite->w ||
		pos.y < -TILE_H16 * sprite->h ||
		pos.x > RENDER_W ||
		pos.y > RENDER_H);
}

void remove_sprite(Sprite* sprite)
{
	printf("Freed sprite: %d\n", sprite->index);
	sprites[sprite->index] = NULL;
	sprite->index = -1;
}

void check_sprite_free(Sprite* sprite)
{
	if (sprite_off_screen(sprite)) remove_sprite(sprite);
}

void draw_sprites()
{
	for (int i = 0; i < SPRITE_COUNT; ++i)
	{
		if (sprites[i] != NULL) draw_sprite(sprites[i]);
	}
}

void update_sprites()
{
	float delta = GetFrameTime();

	for (int i = 0; i < SPRITE_COUNT; ++i)
	{
		Sprite* sprite = sprites[i];
		if (sprite != NULL)
		{
			// animate 
			sprite->frame += SPRITE_ANIM_RATE * delta;

			// move
			if (sprite->moving)
			{
				float diff_x = sprite->move_x - sprite->x;
				float diff_y = sprite->move_y - sprite->y;
				float move = delta * SPRITE_MOVE_SPEED;
				float sign_x = diff_x == 0 ? 0 : diff_x > 0 ? 1 : -1;
				float sign_y = diff_y == 0 ? 0 : diff_y > 0 ? 1 : -1;
				if (abs(diff_x) <= move && abs(diff_y) <= move) 
				{
					sprite->moving = false;
					sprite->x = sprite->move_x;
					sprite->y = sprite->move_y;
				}
				else
				{
					sprite->x += sign_x * move;
					sprite->y += sign_y * move;
				}
			}

			// free sprite if it goes off-screen
			check_sprite_free(sprite);
		}
	}
}
