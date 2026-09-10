
#include "includes.h"
#include "text.h"
#include "command.h"
#include "window.h"
#include "message.h"
#include "event.h"
#include "map_editor.h"
#include "player.h"

char* cmd_string[CMD_COUNT] = {
	"@#$%@",				// to make sure nothing matches it
	TCMD_STEP,
	TCMD_HELLO,
	TCMD_ASK,
	TCMD_CHECK,
	TCMD_TAKE,
	TCMD_BREATHE,
	TCMD_CHANT,
	TCMD_STAIRS,
	TCMD_SHOUT,
	TCMD_NAP,
	TCMD_ATTACK,
	TCMD_GIVE,
	"^^DEFAULT ",			// to make sure nothing matches it
	TCMD_HOTKEY,
	TCMD_EDIT,
	TCMD_BATTLE
};

struct CommandResponse commands[COMMAND_COUNT] = {
	{
		.command = CMD_ATTACK,
		.context = COMMAND_BATTLE,
		.response_type = RESPONSE_FUNCTION,
		.response.function.callback = _command_attack
	},
	{
		.command = CMD_BATTLE,
		.context = COMMAND_MAP | COMMAND_DEBUG,
		.response_type = RESPONSE_FUNCTION,
		.response.function.callback = _command_battle
	},
	{
		.command = CMD_EDIT,
		.context = COMMAND_MAP,
		.response_type = RESPONSE_FUNCTION,
		.response.function.callback = start_map_editor
	},
	{
		.command = CMD_SHOUT,
		.context = COMMAND_MAP,
		.response_type = RESPONSE_FUNCTION,
		.response.function.callback = _command_shout
	},
	{
		.command = CMD_HOTKEY,
		.response_type = RESPONSE_FUNCTION,
		.response.function.callback = _command_hotkey
	}
};

const Rectangle COMMAND_WINDOW_R = { COMMAND_WINDOW };
unsigned char cmd_buffer[CMD_BUFFER_SIZE] = {0};
unsigned char cmd_next = 0;

int match_command(char* cmd)
{
	for (int i = 0; i < CMD_COUNT; ++i)
	{
		if (strmatch(cmd_string[i], cmd)) return i;
	}
	return CMD_NONE;
}

void execute_command_response(int event, int cmd, char* arg, CommandResponse* response)
{
	printf("%s\n", cmd_string[cmd]);
	switch (response->response_type)
	{
		case RESPONSE_MESSAGE:
			message_box(response->response.message.text);
			break;
			
		case RESPONSE_XFER:
			
			break;

		case RESPONSE_ITEM:

			break;

		case RESPONSE_BATTLE:

			break;

		case RESPONSE_SHOP:

			break;

		case RESPONSE_DESTROY:

			break;

		case RESPONSE_FUNCTION:
			response->response.function.callback(event, cmd, arg, response);
			break;
	}
}

void attempt_command(int event, char* cmd, char* arg)
{
	// first check if any command is recognized.
	int command = match_command(cmd);
	
	bool default_available = true;

	if (command != CMD_NONE)
	for ( int i = 0; i < COMMAND_COUNT; ++i)
	{
		struct CommandResponse* response = &commands[i];
		char* r_arg = response->arg ? response->arg : ""; // Since arg is an optional data point.
														  //
		// TODO: check command flags too (COMMAND_MAP and so on)
		// TODO: consolidate this code and the stuff below?
		if (response->command == command && strmatch(r_arg, arg))
		{
			default_available = false;
			execute_command_response(event, command, arg, response);
			break;
		}
	}
		
	if (event == -1) return;

	Event* eobj = &events[map_index][event];
	
	struct CommandResponse* selected_behavior = NULL;
	struct CommandResponse* default_behavior = NULL;
	for ( int i = 0; i < EVENT_BEHAVIORS; ++i)
	{
		struct CommandResponse* behavior = &eobj->behavior[i];
		char* r_arg = behavior->arg ? behavior->arg : ""; // Since arg is an optional data point.
														  //
		if (command != CMD_NONE && behavior->command == command && strmatch(r_arg, arg))
		{
			selected_behavior = behavior;
		}
		if (behavior->command == CMD_DEFAULT)
		{
			default_behavior = behavior;
		}
	}
	// now select the default behavior if nothing else worked.
	if (command != CMD_STEP && default_available && selected_behavior == NULL && default_behavior != NULL) 
	{
		command = CMD_DEFAULT;
		selected_behavior = default_behavior;
	}
	
	// trigger the selected behavior.
	if (selected_behavior) execute_command_response(event, command, arg, selected_behavior);
}

void command_window()
{	
	bool go = true;
	bool blink = true;
	bool execute = false;
	int timer = 30;
	
	draw_window(COMMAND_WINDOW, "Say");
	
	while (go)
	{
		tilebuff[128]=128;
		if (IsKeyPressed(KEY_ESCAPE))
		{
			go = false;
		}
		if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))
		{
			// check for a hotkey command
			for (char c = KEY_A; c <= KEY_Z; ++c)
			{
				if (IsKeyPressed(c))
				{
					strupper(&c);
					strcpy(player.hotkey[c - 65], cmd_buffer);
					blink = false;
					go = false;
					execute = true;
					break; // we don't need anything else from this loop.
				}
			}
		}

		char c;
		while ( (c = GetCharPressed()) != 0 ) 
		{		
			// normal input
			if (cmd_next < CMD_BUFFER_SIZE-2)
			{
				cmd_buffer[cmd_next++] = c;
			}
			else
			{
				memcpy(cmd_buffer, cmd_buffer+1, CMD_BUFFER_SIZE-2);
				cmd_buffer[cmd_next-1] = c;
			}
			// reset timer
			blink = true;
			timer = 30;
		}
		if (cmd_next > 0 && (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)))
		{
			tilebuff_set( 993, 5+cmd_next, 6);
			cmd_next--;
			cmd_buffer[cmd_next] = ' ';
			// reset timer
			blink = true;
			timer = 30;
		}
		if (IsKeyPressed(KEY_ENTER))
		{
			blink = false;
			go = false;
			execute = true;
		}
		timer--;
		if (timer == 0)
		{
			timer = 30;
			blink = !blink;
		}
		tilebuff_print(cmd_buffer, 5, 6);
		tilebuff_set( blink ? 995 : 993, 5+cmd_next, 6);
		render();
	}
	
	if (execute)
	{
		render_and_wait(5);
		execute_cmd_buffer();
	}

	reset_cmd_buffer();
	draw_window_wipe(COMMAND_WINDOW);
	render();
}

void execute_cmd_buffer()
{
	strupper(cmd_buffer);
	check_event_command(
		cmd_buffer, 
		strpbrk(cmd_buffer, " ")+1			// pointer to the position after the first space.
	); 
}

void reset_cmd_buffer()
{
	memset(cmd_buffer, ' ', CMD_BUFFER_SIZE-2);
	cmd_next = 0;
}
