
#ifndef CONST_H
#define CONST_H

#define MAP_FNAME "map_data.bin"
#define MAP_SIZE 8
#define EVENT_SIZE 128
#define EVENT_BEHAVIORS 4

// Player

#define FLAGS 256
#define FLAG_T char
#define FLAG_CHUNK sizeof(FLAG_T)
#define FLAG_N FLAGS / FLAG_CHUNK

#define CMD_BUFFER_SIZE 28 				// arbitrary length fits to window size.
#define STR_NAME_LEN 20					// length of names for battlers

#define SCROLL_OFFSET 24

#define PLAYER_SPEED_NORMAL 1.f
#define PLAYER_SPEED_MEDIUM 0.85f
#define PLAYER_SPEED_SLOW 0.75f

#define MESSAGE_WINDOW 4, 20, 30, 10 	// arguments for window dimensions.
#define MESSAGE_WINDOW_WAIT 1007 		// tile to print for "continue" arrow 
#define COMMAND_WINDOW 4, 4, 30, 4

// TODO: remove?
// enemy-focused command definitions.
// (player can use them too!)
#define CMD_CHANT_POLLEN "CHANT POLLEN"

// Tilemap

#define TILE_W 8
#define TILE_H 8
#define TILE_W16 TILE_W*2
#define TILE_H16 TILE_H*2
#define TILESHEET_W 32
#define TILESHEET_H 32
#define TILESHEET_W16 16
#define TILESHEET_H16 16
#define TILESHEET_SIZE TILESHEET_W * TILESHEET_H
#define TILESHEET_SIZE16 TILESHEET_W16 * TILESHEET_H16
#define TILESHEET_WPX TILESHEET_W * TILE_W
#define TILESHEET_HPX TILESHEET_H * TILE_H
#define TILESHEET_FNAME "tileset.png"
#define TILEMAP_W 128									// tilemap only addresses 16x16 tiles, not 8x8. I could change that? TODO: metatiles?
#define TILEMAP_H 128
#define TILEMAP_SIZE TILEMAP_W * TILEMAP_H
// tile buffer parameters
#define TILEBUFF_W 64							
#define TILEBUFF_H 64
#define TILEBUFF_W16 32
#define TILEBUFF_H16 32
#define TILEBUFF_SIZE TILEBUFF_W * TILEBUFF_H

// Render

#define RENDER_W 256
#define RENDER_H 224

// Window

#define STRBUFF 64
#define DEFAULT_WINDOW_W 1024
#define DEFAULT_WINDOW_H 768
#define WINDOW_TITLE "New Sun"
#define WINDOW_FLAGS FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE
#define GAMEFPS 60
#define FRAMESEC (1.f/GAMEFPS)

#endif
