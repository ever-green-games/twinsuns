
#ifndef BATTLE_HEAD
#define BATTLE_HEAD

#include "util.h"
#include "command.h"

enum StatusEffect
{
	STATUS_SLEEP,
	STATUS_POISON,
	STATUS_NOSPELL,
	STATUS_COUNT
};

typedef struct 
{
	char name[STR_NAME_LEN]; 

	uchar hpmax;
	uchar mpmax;
	uchar hp;
	uchar mp;
	uchar strength;
	uchar agility; 			// this is compared against the player for extra turns and ambush/surprise
	uchar voice;

	uchar xp;
	uchar gold;

	bool status[STATUS_COUNT]; 				// status effect applied
	uchar status_turns[STATUS_COUNT];		// turns of status transpired
} 
BattleData;

typedef struct
{
	BattleData battler;
	uchar tile;
	uchar tile_w;
	uchar tile_h;	
	struct CommandResponse behavior[4];
	char* commands[4];
	// TODO: palette index?
}
EnemyData;

enum EnemyName 
{
	ENEMY_COB_MANTIS,
	ENEMY_COUNT
};

extern EnemyData enemy_data[ENEMY_COUNT];

char* enemy_choose_command(EnemyData* battler);
int battler_defense(BattleData* battler);
int calculate_damage(BattleData* attacker, BattleData* defender);
void battle(EnemyData* enemy);

#endif
