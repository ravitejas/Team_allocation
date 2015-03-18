// SpTeamGame.cpp : Defines the entry point for the console application.

#include <string>
#include <vector>
#include "GameStructs.h"
#include "FunctionLibrary.h"

using namespace std;
using namespace suckerPunchInternTest;

int main(int argc, char* argv[])
{
	string inputFileName;
	string outputFileName = "units_distributed_in_teams.txt";
	float acceptableErrorPercentage = 10.0f;
	int returnStatus = 0;
	returnStatus = readCommandLineInput(argc, argv, inputFileName, outputFileName, acceptableErrorPercentage);
	if (returnStatus != 0)
	{
		return 1;
	}

	vector<BattleUnit> battleUnitList;
	int numberOfTeams = 0;
	int totalPowerOfAllUnits = 0;
	returnStatus = storeTeamsAndUnitsData(inputFileName, battleUnitList, numberOfTeams, totalPowerOfAllUnits);	
	if (returnStatus != 0)
	{
		return 1;
	}
	if (0 == numberOfTeams)
	{
		cout << "Number of teams must be at least 1 \n";
		return 1;
	}
	if (0 == battleUnitList.size())
	{
		cout << "Number of units must be at least 1 \n";
		return 1;
	}

	vector<Team> teamList;
	initializeTeams(numberOfTeams, teamList);

	int targetPowerPerTeam = totalPowerOfAllUnits / numberOfTeams;
	int acceptableError = static_cast<int>(acceptableErrorPercentage * targetPowerPerTeam / 100);
	assignUnitsToTeams(targetPowerPerTeam, acceptableError, battleUnitList, teamList);
	writeTeamsInfoToFile(outputFileName, teamList);

	return 0;
}

