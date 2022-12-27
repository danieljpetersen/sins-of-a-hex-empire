#ifndef NATION_H
#define NATION_H

#include <vector>
#include "Unit.h"

class Game_System;

enum BotState { CaptureNearestCities };

class Nation
{
private:
	bool AIControlled;
	int NumberOfMovesLeft, PlayerIndex;
	BotState AIState;

	int TileAttacking;
	std::vector<int> UnitsSkippingThisTurn;
public:
	Nation();

	std::vector<Unit> Units;

	void setPlayerIndex(int Value);

	bool getAIControlled();
	void setAIControllerd(bool Value);

	void simulateTurn(Game_System *Game);

	void decrementNumberOfMovesLeft();
	void resetNumberOfMovesLeft();

	//======
	void captureNearestCitiesState(Game_System *Game);

	void getUnitsToAttack(Game_System *Game, int &UnitIndex, int &GoalIndex);

	bool canWeWinAttack(Game_System *Game, int TileToAttack);

	void determineNearestCity(Game_System *Game, int &UnitIndex, int &CityIndex, bool LookForFullArmies);

	void aiMoveUnit(Game_System *Game, int UnitIndex, int GoalTile);

	bool isValidArmy(int Index);

	bool areWeSkippingUnit(int Index);
};
 
#endif 