// FunctionLibrary.cpp : Defines various functions used in the program

#include "FunctionLibrary.h"

using namespace suckerPunchInternTest;

void suckerPunchInternTest::assignUnitsToTeams(int targetPowerPerTeam, int acceptableError, const vector<BattleUnit>& battleUnitList, vector<Team>& teamList)
{
	cout << "Assigning units to teams. Parameters: \n";
	cout << "   Target Power per team = " << targetPowerPerTeam << "\n";
	cout << "   Acceptable Error = " << acceptableError << "\n";

	vector<BattleUnit> sortedBattleUnitList = battleUnitList;
	std::sort(sortedBattleUnitList.begin(), sortedBattleUnitList.end(), isUnitMorePowerful);

	vector<BattleUnit>::const_iterator unitIterator;
	vector<Team>::iterator teamIterator = teamList.begin();

	//Every iteration, one unit will be assigned to a team.
	//if the current team cannot accept the unit, the next team will be checked
	//if a unit cannot be added to any of the teams, it will be forcibly added to the last team	
	for (unitIterator = sortedBattleUnitList.begin(); unitIterator != sortedBattleUnitList.end(); unitIterator++)
	{
		while (!shouldAddUnitToTeam(*unitIterator, *teamIterator, targetPowerPerTeam, acceptableError))
		{
			if (teamIterator == teamList.end() - 1) //last element
			{
				break;
			}
			teamIterator++;
		}
		addUnitToTeam(*unitIterator, *teamIterator);

		//cycle through the teams
		if (teamIterator == teamList.end() - 1)
		{
			teamIterator = teamList.begin();
		}
		else
		{
			teamIterator++;
		}
	}
	rebalanceTheTeams(targetPowerPerTeam, battleUnitList, teamList);
	cout << "Completed assigning units to teams! \n\n";
}

void suckerPunchInternTest::addUnitToTeam(const BattleUnit& unit, Team& team)
{
	team.unitIdList.push_back(unit.id);
	team.totalPower += unit.attackPower + unit.defencePower;
}

//If adding the unit brings the team's totalPower closer to the targetPower,
//without overshooting the targetPower by more than acceptableError,
//then add it.
bool suckerPunchInternTest::shouldAddUnitToTeam(const BattleUnit& unit, const Team& team, int targetPowerPerTeam, int acceptableError)
{
	if (abs(team.totalPower - targetPowerPerTeam) <= acceptableError ||
		team.totalPower + unit.attackPower + unit.defencePower - targetPowerPerTeam > acceptableError)
	{
		return false;
	}
	return true;
}

//The last team may have a very high power due to the greedy algo used
//Distribute units from the last team to other teams
void suckerPunchInternTest::rebalanceTheTeams(int targetPowerPerTeam, const vector<BattleUnit>& battleUnitList, vector<Team>& teamList)
{
	Team& lastTeam = teamList.back();
	int numUnitsInTeam = lastTeam.unitIdList.size();
	for (int i = 1; i <= numUnitsInTeam; i++)
	{
		BattleUnit lastUnit = battleUnitList.at(lastTeam.unitIdList.back());
		int lastUnitPower = lastUnit.attackPower + lastUnit.defencePower;
		Team& teamWithLeastPower = teamList.at(getIndexOfTeamWithLeastPower(teamList));
		
		if (lastTeam.id == teamWithLeastPower.id)
		{
			break; //nothing more to distribute
		}
		int currentImbalance = abs(lastTeam.totalPower - targetPowerPerTeam) + abs(teamWithLeastPower.totalPower - targetPowerPerTeam);
		int imbalanceAfterUnitTransfer = abs(lastTeam.totalPower - lastUnitPower - targetPowerPerTeam) +
			                             abs(teamWithLeastPower.totalPower + lastUnitPower - targetPowerPerTeam);

		if (imbalanceAfterUnitTransfer < currentImbalance) //then transfer the unit
		{
			lastTeam.totalPower -= lastUnitPower;
			lastTeam.unitIdList.pop_back();
			addUnitToTeam(lastUnit, teamWithLeastPower);
		}
		else
		{
			break; //if unable to transfer units, that means optimum balance is reached.
		}
	}
}

int suckerPunchInternTest::getIndexOfTeamWithLeastPower(const vector<Team>& teamList)
{
	int indexOfTeamWithLeastPower = 0;
	int leastPower = teamList.at(0).totalPower;

	for (unsigned int i = 1; i < teamList.size(); i++)
	{
		if (teamList.at(i).totalPower < leastPower)
		{
			indexOfTeamWithLeastPower = i;
			leastPower = teamList.at(i).totalPower;
		}
	}
	return indexOfTeamWithLeastPower;
}

bool suckerPunchInternTest::isUnitMorePowerful(const BattleUnit& unit1, const BattleUnit& unit2)
{
	return (unit1.attackPower + unit1.defencePower) > (unit2.attackPower + unit2.defencePower);
}

void suckerPunchInternTest::initializeTeams(int numberOfTeams, vector<Team>& teamList)
{
	cout << "Creating " << numberOfTeams << " empty teams \n";
	for (int i = 0; i < numberOfTeams; i++)
	{
		Team tempTeam(i);
		teamList.push_back(tempTeam);
	}
}

int suckerPunchInternTest::storeTeamsAndUnitsData(const string& inputFileName, vector<BattleUnit>& battleUnitList, int& numberOfTeams, int& totalPowerOfAllUnits)
{
	cout << "Reading info from file: \"" << inputFileName << "\" \n";
	ifstream inputFileStream(inputFileName);
	if (inputFileStream.is_open())
	{
		string lineFromFile;
		int lineNumber = 1;
		totalPowerOfAllUnits = 0;

		while (getline(inputFileStream, lineFromFile))
		{
			if (1 == lineNumber)
			{
				numberOfTeams = atoi(lineFromFile.c_str());
			}
			else
			{
				int indexOfComma = lineFromFile.find(',');
				if (indexOfComma >= 0)
				{
					int attackPower = atoi(lineFromFile.substr(0, indexOfComma).c_str());
					int defencePower = atoi(lineFromFile.substr(indexOfComma + 1).c_str());
					BattleUnit currentUnit(lineNumber - 2, attackPower, defencePower);
					battleUnitList.push_back(currentUnit);
					totalPowerOfAllUnits += (attackPower + defencePower);
				}
			}
			lineNumber++;
		}
		inputFileStream.close();
		cout << "Completed reading teams and units info \n\n";
	}
	else
	{
		cout << "ERROR: Unable to open file \n\n";
		return -1;
	}
	return 0;
}


int suckerPunchInternTest::readCommandLineInput(int argc, char* argv[], string& inputFileName, string& outputFileName, float& acceptableErrorPercentage)
{
	cout << "Examining input arguments \n";
	if (argc < 2)
	{
		cout << "\nUsage: SpTeamGame input_file_name acceptable_error_percentage output_file_name \n";
		cout << "input_file_name needed \n";
		cout << "acceptable_error_percentage optional. Default: 10. Controls accuracy of team balance \n";
		cout << "output_file_name optional. Default: units_distributed_in_teams.txt \n\n";		
		return -1;
	}
	else
	{
		inputFileName.assign(argv[1]);
		if (argc > 2)
		{
			acceptableErrorPercentage = stof(argv[2]);
		}
		if (argc > 3)
		{
			outputFileName.assign(argv[3]);
		}
		cout << "Successfully read input arguments \n\n";
		return 0;
	}
}

void suckerPunchInternTest::writeTeamsInfoToFile(const string& outputFileName, vector<Team>& teamList)
{
	cout << "Writing results to file: \"" << outputFileName << "\" \n";
	ofstream outputFileStream(outputFileName);
	if (outputFileStream.is_open())
	{
		outputFileStream << "Team_Number --- Units_in_Team --- Team_Power \n\n";
		vector<Team>::const_iterator teamIterator = teamList.begin();
		for (; teamIterator != teamList.end(); teamIterator++)
		{
			Team currentTeam = *teamIterator;
			outputFileStream << currentTeam.id << " --- ";

			unsigned int unitIdIndex = 0;
			for (; unitIdIndex < currentTeam.unitIdList.size() - 1; unitIdIndex++)
			{
				outputFileStream << currentTeam.unitIdList.at(unitIdIndex) << ", ";
			}
			outputFileStream << currentTeam.unitIdList.at(unitIdIndex); //print last unitId without a comma
			outputFileStream << " --- " << currentTeam.totalPower << "\n";
		}
		outputFileStream.close();
		cout << "Writing results completed \n\n";
	}
	else
	{
		cout << "ERROR: Unable to open file \n\n";
	}
}