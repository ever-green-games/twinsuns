
#include "includes.h"

#include "tilemap.h"
#include "sprite.h"
#include "tile_viewer.h"
#include "render.h"
#include "map_editor.h"

RenderTexture2D rtex;
Rectangle rtex_drect;

void init_render()
{
	rtex = LoadRenderTexture(RENDER_W, RENDER_H);
	if (!IsRenderTextureValid(rtex))
	{
		printf("Failed to load render texture.");
		exit(1);
	}
	update_rtex_drect();
}

void update_rtex_drect()
{
	int sh = GetScreenHeight();
	int sw = GetScreenWidth();
	int scale = floor(sh / RENDER_H);
	int w = RENDER_W * scale;
	int h = RENDER_H * scale;
	int x = sw/2 - w/2;
	int y = sh/2 - h/2;

	rtex_drect = (Rectangle){x, y, w, h};
}

void render()
{
	// if the window is resized, update the rendertexture draw rect.
	if (IsWindowResized())
	{
		update_rtex_drect();
	}

	// first render to the render texture
	BeginTextureMode(rtex);

		ClearBackground(BLACK);

		draw_tile_buffer();
		draw_sprites();

		// EDITOR FEATURES
		update_map_editor();

	EndTextureMode();

	// then render that to the screen
	BeginDrawing();

		ClearBackground(BLACK);

		const Rectangle rtex_srect = {0, 0, RENDER_W, -RENDER_H};
		const Vector2 rtex_origin = {0, 0};

		DrawTexturePro(
			rtex.texture,
			rtex_srect,
			rtex_drect,
			rtex_origin,
			0,
			WHITE
		);

		tile_viewer();

	EndDrawing();
}

void render_and_wait(int frames)
{
	render();
	WaitTime(FRAMESEC * (float)frames);
}
