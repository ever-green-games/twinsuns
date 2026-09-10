#include <stdlib.h>
#include <time.h>
#include "const.h"
#include "game.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

int main ()
{
	srand(time(NULL));
	SetConfigFlags( WINDOW_FLAGS );

	InitWindow(DEFAULT_WINDOW_W, DEFAULT_WINDOW_H, WINDOW_TITLE);
	SetTargetFPS(60);

	SearchAndSetResourceDir("resources");
	
	// game.h
	init();

	while (!WindowShouldClose())
	{
		update();
	}

	deinit();
	CloseWindow();
	return 0;
}
