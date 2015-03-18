// FunctionLibrary.h : Declares various functions used in the program

#ifndef FUNCTION_LIBRARY_H
#define FUNCTION_LIBRARY_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "GameStructs.h"

using namespace std;

namespace suckerPunchInternTest
{
	void assignUnitsToTeams(int targetPowerPerTeam, int acceptableError, const vector<BattleUnit>& battleUnitList, vector<Team>& teamList);	
	void addUnitToTeam(const BattleUnit& unit, Team& team);
	bool shouldAddUnitToTeam(const BattleUnit& unit, const Team& team, int targetPowerPerTeam, int acceptableError);
	void rebalanceTheTeams(int targetPowerPerTeam, const vector<BattleUnit>& battleUnitList, vector<Team>& teamList);
	int getIndexOfTeamWithLeastPower(const vector<Team>& teamList);
	bool isUnitMorePowerful(const BattleUnit& unit1, const BattleUnit& unit2);
	void initializeTeams(int numberOfTeams, vector<Team>& teamList);
	int storeTeamsAndUnitsData(const string& inputFileName, vector<BattleUnit>& battleUnitList, int& numberOfTeams, int& totalPowerOfAllUnits);

	int readCommandLineInput(int argc, char* argv[], string& inputFileName, string& outputFileName, float& acceptableErrorPercentage);
	void writeTeamsInfoToFile(const string& outputFileName, vector<Team>& teamList);
}

#endif