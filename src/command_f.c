
#include "includes.h"
#include "command_f.h"
#include "message.h"
#include "player.h"

COMMAND_FDEF(_command_battle)
{
	// TODO: initiate a battle
	
	int index = atoi(arg);
}

COMMAND_FDEF(_command_attack)
{
	// TODO: call function in battle.
}

COMMAND_FDEF(_response_battle)
{
	// TODO: battle(enemy)
}

COMMAND_FDEF(_command_shout)
{
	message_box(arg);
}

COMMAND_FDEF(_command_hotkey)
{
	char hotkeymsg[1024] = {0};
	char* hotkeynext = hotkeymsg;
	for (int i = 0; i < 26; ++i)
	{
		char* hotkey = player.hotkey[i];
		if (hotkey[0] != '\0')
		{
			//sprintf(hotkeymsg, "%s\n%c - %s", hotkeymsg, (char)(i+65), hotkey);
			hotkeynext[0] = i + 65;
			strcat(hotkeynext, " - ");
			strcat(hotkeynext, hotkey); // TODO: why does this print a tree and do weird new lines.
			strcat(hotkeymsg, "\n");
			hotkeynext += 5 + strlen(hotkey);
		}
	}
	message_box(hotkeymsg);
}
