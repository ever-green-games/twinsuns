
#ifndef EVENT_HEAD
#define EVENT_HEAD

#include "util.h"
#include "command.h"

enum EventFlag {
	EVENT_SOLID 	= 1 << 0,
	EVENT_ISCHAR 	= 1 << 1,
	EVENT_WANDERS 	= 1 << 2,
	EVENT_LARGE 	= 1 << 3				// 2x2 tile event.
};

typedef struct {
	uchar flag;
	uchar tile;
	uchar eflags;
	char name[12];
	CommandResponse behavior[EVENT_BEHAVIORS];
} Event;

#define EVENT_XFER(TO_MAP,TO_X,TO_Y,TILE,...) \
	{ \
		.tile = TILE, \
		.behavior = { \
			{ \
				.command = CMD_STEP, \
				.response_type = RESPONSE_XFER, \
				.response.xfer = { \
					.to_map = TO_MAP, \
					.to_x = TO_X, \
					.to_y = TO_Y, \
					.bounds = { __VA_ARGS__ } \
				} \
			} \
		} \
	} \

extern Event events[MAP_SIZE][EVENT_SIZE];

#endif
