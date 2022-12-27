#include "Pathfinding.h"
#include "Board.h"
#include "Application.h"

void Pathfinding::setBoardPointer(Board_Component *BoardObject)
{
	TheBoard = BoardObject;
}

void Pathfinding::resetAllVariables()
{
	for (int i = 0; i < NUMBER_OF_TILES; i++)
		for (int j = 0; j < NUMBER_OF_TILES; j++)
			TurnsToReachGoal[i][j] = -1;	

	for (int i = 0; i < NUMBER_OF_TILES; i++)
	{
		WasTileAnalysed[i] = false;
		IsTileOnTilesToAnalyseList[i] = false;
	}

	TilesToAnalyse.clear();
	StartTileIndex = 0;
	GoalTileIndex = 0;
	CurrentScore = 0;
	ExaminingThisTile = 0;
}

void Pathfinding::analyseBoard()
{	
	this->resetAllVariables();
	bool Continue = true;

	while (Continue == true)
	{
		Continue = this->addTilesToAnalyse();
		if (Continue == false)
			break;

		this->setScoreForTilesToAnalyse();
		this->addTilesToAnalyse();
		this->removeTilesAnalysed();		
		this->setScoreForTilesToAnalyse();
	}
}

bool Pathfinding::addTilesToAnalyse()
{
	//tiles that were analysed get their neighbors which weren't yet analysed added to the list
	if (TilesToAnalyse.empty() != true)
	{
		for (std::list<int>::iterator iter = TilesToAnalyse.begin(); iter != TilesToAnalyse.end(); ++iter) 
			if (WasTileAnalysed[(*iter)] == true)
			{
				int Neighbor[6];
				TheBoard->getHexNeighbors((*iter), Neighbor);

				for (int i = 0; i < NUMBER_OF_ADJACENT_TILES; i++)
					if (TheBoard->isValidTile(Neighbor[i]) == true)
						if (WasTileAnalysed[Neighbor[i]] != true)
							if (TheBoard->Tiles[Neighbor[i]].getType() != WATER)
								if (IsTileOnTilesToAnalyseList[Neighbor[i]] != true)
								{
									TilesToAnalyse.push_back(Neighbor[i]);
									IsTileOnTilesToAnalyseList[Neighbor[i]] = true;
								}
			}
	}
	else //if there are no tiles in the queue to analyse, we're on a new starting tile and need to act accordingly
	{
		ExaminingThisTile = StartTileIndex;
		TilesToAnalyse.push_back(StartTileIndex++);

		while ((TheBoard->isValidTile(StartTileIndex)) && (TheBoard->Tiles[StartTileIndex].getType() == WATER))
		{
			StartTileIndex++;
		}
			
		//if we've went through every tile, return false
		if (TheBoard->isValidTile(TilesToAnalyse.back()) != true)
			return false;
		
		CurrentScore = 0;
		for (int i = 0; i < NUMBER_OF_TILES; i++)
		{
			WasTileAnalysed[i] = false;	
			IsTileOnTilesToAnalyseList[i] = false;
		}
	}

	return true;
}

void Pathfinding::setScoreForTilesToAnalyse()
{
	if (TilesToAnalyse.empty() != true)
	{
		for (std::list<int>::iterator iter = TilesToAnalyse.begin(); iter != TilesToAnalyse.end(); ++iter) 
			if (WasTileAnalysed[(*iter)] == false)
			{
				WasTileAnalysed[(*iter)] = true;
				TurnsToReachGoal[ExaminingThisTile][(*iter)] = CurrentScore;
			}
	}

	CurrentScore++;
}

void Pathfinding::removeTilesAnalysed()
{
	if (TilesToAnalyse.empty() != true)
	{
		for (std::list<int>::iterator iter = TilesToAnalyse.begin(); iter != TilesToAnalyse.end(); /**/) 
		{
			if (WasTileAnalysed[(*iter)] == true)
				TilesToAnalyse.erase(iter++);
			else
				iter++;
		}
	}
}

int Pathfinding::getTurnsToReachGoal(int StartingTile, int EndingTile)
{
	return TurnsToReachGoal[StartingTile][EndingTile];
}

int Pathfinding::findNextTileToMoveTo(int Start, int Goal) 
{
	int Neighbor[NUMBER_OF_ADJACENT_TILES];
	TheBoard->getHexNeighbors(Start, Neighbor);
	
	int LowestScore = -99;
	for (int i = 0; i < NUMBER_OF_ADJACENT_TILES; i++)
	{
		if (TheBoard->isValidTile(Neighbor[i]) == true)
		{
			if (TheBoard->Tiles[Neighbor[i]].getType() != WATER)
			{
				if (Neighbor[i] == Goal)
					return Goal;

				if (LowestScore == -99)
					LowestScore = Neighbor[i];
				else if (this->getTurnsToReachGoal(Neighbor[i], Goal) < this->getTurnsToReachGoal(LowestScore, Goal))
						LowestScore = Neighbor[i];

			}
		}
	}

	return LowestScore;
}

bool Pathfinding::checkIfValidAndFinalizeBoard()
{
	//if a board has any starting areas which are not accessible to all other starting areas, it is invalid
	//if it's invalid, return false and the function that calls this generates a new board
	//if it's valid, all land tiles inaccessible are turned into water at the end of this function
	bool ValidBoard = true;
	if (this->getTurnsToReachGoal(TOP_LEFT_CAPITAL, BOTTOM_LEFT_CAPITAL) == -1)
		ValidBoard = false;
	if (this->getTurnsToReachGoal(TOP_LEFT_CAPITAL, TOP_RIGHT_CAPITAL) == -1)
		ValidBoard = false;
	if (this->getTurnsToReachGoal(TOP_LEFT_CAPITAL, BOTTOM_RIGHT_CAPITAL) == -1)
		ValidBoard = false;

	if (ValidBoard != true)
		return false;
	
	//make any unreachable tile water + count Num of cities
	TheBoard->CityTiles.clear();
	TheBoard->CityTiles.resize(0);
	for (int i = 0; i < NUMBER_OF_TILES; i++)
	{
		if (this->getTurnsToReachGoal(TOP_LEFT_CAPITAL, i) == -1)
			TheBoard->Tiles[i].setType(WATER);

		if ((TheBoard->Tiles[i].getType() == CITY) || (TheBoard->Tiles[i].getType() == CAPITAL))
			TheBoard->CityTiles.push_back(i);
	}

	return true;
}