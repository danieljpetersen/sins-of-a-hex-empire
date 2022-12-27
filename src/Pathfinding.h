#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "Constants.h"
#include <list>
class Board_Component;

//Brief explanation for how this works 
//The array TurnsToReachGoal contains the number of turns to reach a given tile 
//First dimension is the start tile, second is the goal tile, and the array is how many turns it takes to go from one to another
//If you want to find a path, you would start at the desired goal and look at all adjacent hexes and pick the lowest number.
//Keep doing this until you reach the starting position, and you now have the shortest path.
//When The player picks a nation the map is analyzed and these routes created

class Pathfinding 
{
private:
	Board_Component *TheBoard;

	static const int NUMBER_OF_TILES = 20 * 11;
	int TurnsToReachGoal[NUMBER_OF_TILES][NUMBER_OF_TILES];//first dimension is the starting tile, second is the goal tile

	//everything private below is for initializing TurnsToReachGoal
	bool WasTileAnalysed[NUMBER_OF_TILES];
	bool IsTileOnTilesToAnalyseList[NUMBER_OF_TILES]; 
	std::list<int>TilesToAnalyse;

	int StartTileIndex, GoalTileIndex, ExaminingThisTile;
	int CurrentScore;
	
	void resetAllVariables(); 

	bool addTilesToAnalyse();
	void setScoreForTilesToAnalyse();
	void removeTilesAnalysed();

public:
	void setBoardPointer(Board_Component *BoardObject);

	void analyseBoard(); //only should be called once per map
	bool checkIfValidAndFinalizeBoard();

	int findNextTileToMoveTo(int Start, int Goal); 
	int getTurnsToReachGoal(int StartingTile, int EndingTile);
};

#endif