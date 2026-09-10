
#ifndef COMMAND_F_H
#define COMMAND_F_H

typedef struct CommandResponse CommandResponse; // TODO: fix double typedef?

#define COMMAND_FDEF(funcname) void funcname(int event, int cmd, char* arg, CommandResponse* response)

COMMAND_FDEF(_command_battle);
COMMAND_FDEF(_command_attack);
COMMAND_FDEF(_response_battle);
COMMAND_FDEF(_command_shout);
COMMAND_FDEF(_command_hotkey);

#endif
