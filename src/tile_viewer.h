
#ifndef TILE_VIEWER_HEAD
#define TILE_VIEWER_HEAD

#include "raylib.h"

// TODO: make debug only.

bool show_tile_viewer = false;

void tile_viewer()
{
	if (IsKeyPressed(KEY_F1)) show_tile_viewer = !show_tile_viewer;

	if (show_tile_viewer)
	{
		Vector2 mouse_pos = GetMousePosition();
		int tile = floor(mouse_pos.x / TILE_W) + floor(mouse_pos.y / TILE_H) * TILESHEET_W;
		char text[64];
		sprintf(text, "%d", tile);

		DrawTexture(tileset, 0, 0, WHITE);
		DrawText(text, 0, TILESHEET_HPX+32, 12, WHITE);

		int tile16 = floor(mouse_pos.x / 16) + floor(mouse_pos.y / 16) * (TILESHEET_W/2);
		sprintf(text, "%d", tile16);
		DrawText(text, 0, TILESHEET_HPX+48, 12, WHITE);
	}
}

#endif
