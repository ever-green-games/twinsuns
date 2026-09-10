#include "includes.h"
#include "window.h"
#include "message.h"

const Rectangle MESSAGE_WINDOW_R = { MESSAGE_WINDOW }; // const.h
const int message_speeds[3] = { 1, 2, 3 };
int message_speed = 1;


void message_box_wait()
{
	bool go = true;
	tilebuff_set( 
		MESSAGE_WINDOW_WAIT,
		MESSAGE_WINDOW_R.x + floor(MESSAGE_WINDOW_R.width/2)-1,
		MESSAGE_WINDOW_R.y + MESSAGE_WINDOW_R.height-1
	);
	render_and_wait(1);
	while (go)
	{
		// TODO: whatever the interact key is.
		if (GetKeyPressed())
		{
			go = false;
		}
		render();
	}
	tilebuff_set( 
		window_tile[7],
		MESSAGE_WINDOW_R.x + floor(MESSAGE_WINDOW_R.width/2)-1,
		MESSAGE_WINDOW_R.y + MESSAGE_WINDOW_R.height-1
	);
}

void message_box(char* text)
{
	const int LINE_MAX = MESSAGE_WINDOW_R.width - 2;

	draw_window(MESSAGE_WINDOW, NULL);
	bool go = true;
	int xstart = MESSAGE_WINDOW_R.x+1;
	int x = xstart;
	int y = MESSAGE_WINDOW_R.y+1;
	int i = 0; // position of printing text.
	int ln = 0; // position of last new line.
	bool newln = false;
	
	while (go)
	{
		bool print = true;
		char c = text[i];
		
		// check for print end
		if (c == '\0') 
		{	
			// no more print
			message_box_wait();
			go = false;
			continue;
		}
		// check for new line
		if (c == '\n') 
		{
			newln = true;
			print = false;
		}
		// check for text pause
		if (c == '@') 
		{
			message_box_wait();
			continue;
		}
		// check for word wrapping on each space.
		if (c == ' ')
		{
			for (int j = i; j < strlen(text); ++j)
			{
				// check for word wrap
				if (j - ln > LINE_MAX)
				{
					newln = true;
					break;

					// TODO: add procedure for scrolling text.
				}
				if (i != j && text[j] == ' ') break; // we've reached a new word without needing to word wrap.
			}
		}

		if (newln)
		{
			i++;
			ln = i;
			y++;
			x = xstart;
			newln = false;
		}

		if (print)
		{
			// now print the new character and pause.
			tilebuff_set( char_to_tile(text[i]), x, y);
			x+=1;
			i++;
			render_and_wait(message_speed);
		}
	}
	draw_window_wipe(MESSAGE_WINDOW);
	render_and_wait(1);
}
