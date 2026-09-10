
#include "includes.h"
#include "text.h"
#include "command.h"
#include "battle.h"

EnemyData enemy_data[ENEMY_COUNT] = {
	// ENEMY_COB_MANTIS
	{
		.battler = {	
			.name = "Cob Mantis",
			.hp = 10,
			.strength = 7,
			.agility = 10,
			.xp = 5,
			.gold = 3,
		},
		.tile = 76,
		.tile_w = 2,
		.tile_h = 2,
		.behavior = {
			{
				.command=CMD_CHANT,
				.arg="INFERNUS ",			// TODO: macro
				.response_type = RESPONSE_POTENCY,
				.response.potency.scalar = 2.f
			}
		},
		.commands = {
			TCMD_ATTACK,
			CMD_CHANT_POLLEN
		}
	}
};

char* enemy_choose_command(EnemyData* enemy)
{
	int choice = fmax(0, 3 - (rand() % 2) - (rand() % 2)); // TODO: test/verify this random distribution?
	while (enemy->commands[choice] == NULL && choice > 0) choice--;
	return enemy->commands[choice];
}

int battler_defense(BattleData* battler)
{
	return battler->agility / 2;
}

int calculate_damage(BattleData* attacker, BattleData* defender)
{
	return attacker->strength * 2 - battler_defense(defender);
}

// TODO: ???
void battle(EnemyData* enemy)
{
	bool is_battle = true;
	// open battle window
	
	// open dialog window
	
	// check for ambush/surprise
	
	// wait for player command (loop)
}
