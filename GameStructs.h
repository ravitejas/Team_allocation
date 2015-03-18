//GameStructs.h : Defines Structures used in program

#ifndef GAME_STRUCTS_H
#define GAME_STRUCTS_H

#include <vector>

namespace suckerPunchInternTest
{
	struct BattleUnit
	{
		int id;
		int attackPower;
		int defencePower;

		BattleUnit(int _id, int _attackPower, int _defencePower) :
			id(_id),
			attackPower(_attackPower),
			defencePower(_defencePower)
		{
		}		
	};	

	struct Team
	{
		int id;
		std::vector<int> unitIdList;
		int totalPower;

		Team(int _id) :
			id(_id)
		{
			totalPower = 0;
		}
	};
}
#endif