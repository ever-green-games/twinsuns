
#ifndef WINDOW_HEAD
#define WINDOW_HEAD

extern const tile_t window_tile[9];

void draw_window(int x, int y, int w, int h, char* title);
void draw_window_wipe(int x, int y, int w, int h);

#endif
