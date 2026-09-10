
#include "includes.h"

#include "tilemap.h"
#include "render.h"
#include "map.h"
#include "window.h"

const tile_t window_tile[9] = {
	998,  999,  1000,
	1001, 1002, 1003,
	1004, 1005, 1006
};

// put a window into the tile buffer
void draw_window(int x, int y, int w, int h, char* title)
{
	// draw the top 
	tilebuff_set(window_tile[0], x, y);
	for (int i = x+1; i < x+w-1; ++i) tilebuff_set(window_tile[1], i, y);
	tilebuff_set(window_tile[2], x+w-1, y);
	if (title != NULL) tilebuff_print(title, x+1, y);

	// pause
	render_and_wait(1);

	// draw the middle
	for (int yy = y+1; yy < y+h-1; ++yy) 
	{
		tilebuff_set(window_tile[3], x, yy);
		for (int xx = x+1; xx < x+w-1; ++xx) tilebuff_set(window_tile[4], xx, yy);
		tilebuff_set(window_tile[5], x+w-1, yy);
	}

	// pause
	render_and_wait(1);

	// draw the bottom
	tilebuff_set(window_tile[6], x, y+h-1);
	for (int xx = x+1; xx < x+w-1; ++xx) tilebuff_set(window_tile[7], xx, y+h-1);
	tilebuff_set(window_tile[8], x+w-1, y+h-1);

	// pause
	render_and_wait(1);
}

// animation to clear a window area with map tlies.
void draw_window_wipe(int x, int y, int w, int h)
{
	// convert to map coords.
	x/=2;
	y/=2;
	w/=2;
	h/=2;
	for(int yy = y+h; yy >= y; --yy)
	{
		for(int xx = x; xx < x+w; ++xx)
		{
			map_to_tilebuff_tile(xx, yy);
		}
		render_and_wait(2);
	}	
}
