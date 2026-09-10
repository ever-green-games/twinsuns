
#include "includes.h"
#include "event.h"

Event events[MAP_SIZE][EVENT_SIZE] = {
	// OVERWORLD
	{
		{
			.flag = 1,
			.eflags = EVENT_SOLID | EVENT_ISCHAR,
			.tile = 128,
			.name = "Tester",
			.behavior = {
				{
					.command = CMD_HELLO,
					.arg = "MISS",
					.response_type = RESPONSE_MESSAGE,
					.response.message = { "Finally somebody called me 'Miss'!" }
				},
				{
					.command = CMD_DEFAULT,
					.response_type = RESPONSE_MESSAGE,
					.response.message = { "Don't just gawk at me!" }
				},
				{
					.command = CMD_STEP,
					.response_type = RESPONSE_MESSAGE,
					.response.message = { "Don't step on me!" }
				}
			}
		},
		EVENT_XFER(1, 0, 0, 0, 0, 0, 16, 16)
	},
	// TOWNS 1
	{
		// OIL MOTHER
		{
			.tile = 144,
			.eflags = EVENT_SOLID | EVENT_LARGE,
			.behavior = {
				{
					.command = CMD_HELLO,
					.response_type = RESPONSE_MESSAGE,
					.response.message = { "Remember that no matter what, I am here for you, %n.@\n\nTo reach your next level, %x XP is needed.\n\nGo forth my child, and may the world welcome you." } 	
				},
				{
					.command = CMD_DEFAULT,
					.response_type = RESPONSE_MESSAGE,
					.response.message = { "When you're not sure what to say to someone, a simple 'Hello' might surprise you.\n@I know you're ready for the world, my dear." }
				}
			}
		}
	}
};
