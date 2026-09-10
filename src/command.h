
#ifndef COMMAND_H
#define COMMAND_H

typedef struct CommandResponse CommandResponse;
typedef void (command_func)(int, int, char*, CommandResponse* response);

#include "tilemap.h"
#include "command_f.h"

enum Command {
	CMD_NONE,
	CMD_STEP,
	CMD_HELLO,
	CMD_ASK,
	CMD_CHECK,
	CMD_TAKE,
	CMD_BREATHE,
	CMD_CHANT,
	CMD_STAIRS,
	CMD_SHOUT,
	CMD_NAP,
	CMD_ATTACK,
	CMD_GIVE,
	CMD_DEFAULT,
	CMD_HOTKEY,
	CMD_EDIT,
	CMD_BATTLE,
	CMD_COUNT
};

extern char* cmd_string[CMD_COUNT];

enum ResponseType {
	RESPONSE_NONE,
	RESPONSE_MESSAGE,
	RESPONSE_XFER,
	RESPONSE_ITEM,
	RESPONSE_BATTLE,
	RESPONSE_SHOP,
	RESPONSE_DESTROY,
	RESPONSE_FUNCTION,
	RESPONSE_POTENCY,
	RESPONSE_COUNT
};

struct MessageResponse {
	char* text;
};

struct XferResponse {
	uchar to_map;
	uchar to_x;
	uchar to_y;
	bool has_bounds;					// this is used to "clip" the map and hide space outside the specified bounds. TODO: implement.
	trec bounds;						// if the current map and x/y are the same, then it will just set the bounds. 
										// the "hidden" space renders the default/outer tile.
};

struct ItemResponse {
	uchar item;
	uchar flag;
};

struct BattleResponse {
	uchar enemy;
	uchar flag;
};

struct ShopResponse {
	char* text;
	uchar shop_id;
};

struct DestroyResponse {
	uchar item; 					// if it requires an item. 0 = no.
	uchar flag;
};

struct FunctionResponse {
	command_func* callback; // function response - receives event id, command name and arg.
};

// TODO: implement functionality (alongside battle.)
struct PotencyResponse {
	float scalar;						// multiplier value. used for enemies.
};

union Response {
	struct MessageResponse message;
	struct XferResponse xfer;
	struct ItemResponse item;
	struct BattleResponse battle;
	struct ShopResponse shop;
	struct DestroyResponse destroy;
	struct FunctionResponse function;
	struct PotencyResponse potency;
};

// a flag for where the command applies, ONLY for commands that act without an event target.
// TODO: implement listening to this.
enum CommandCtx
{
	COMMAND_ANY,
	COMMAND_MAP 		= 1 << 0,
	COMMAND_BATTLE		= 1 << 1,
	COMMAND_DEBUG		= 1 << 2
};

struct CommandResponse {
	int command;
	char* arg;
	enum CommandCtx context; 
	enum ResponseType response_type;
	union Response response;
};

#define COMMAND_COUNT 5
extern struct CommandResponse commands[COMMAND_COUNT];

extern const Rectangle COMMAND_WINDOW_R;
extern unsigned char cmd_buffer[CMD_BUFFER_SIZE];
extern unsigned char cmd_next;

int match_command(char* cmd);
void execute_command_response(int event, int cmd, char* arg, CommandResponse* response);
void attempt_command(int event, char* cmd, char* arg);
void command_window(); 
void execute_cmd_buffer();		// format the cmd_buffer and translate it into attempt_command()
void reset_cmd_buffer();

#endif
