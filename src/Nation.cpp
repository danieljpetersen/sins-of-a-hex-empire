#include "Nation.h"
#include "Game.h"
#include "Application.h"

Nation::Nation()
{
	Units.resize(0);
	AIControlled = true;
	NumberOfMovesLeft = 5;
	AIState = CaptureNearestCities;
	PlayerIndex = 0;
	TileAttacking = -1;
}

//==================================

bool Nation::getAIControlled()
{
	return AIControlled;
}

//==================================

void Nation::setAIControllerd(bool Value)
{
	AIControlled = Value;
}

//==================================
void Nation::decrementNumberOfMovesLeft()
{
	NumberOfMovesLeft--;
	if (NumberOfMovesLeft == 0)
		for (unsigned int i = 0; i < Units.size(); i++)
			Units[i].setDidUnitMove(true);
}

void Nation::resetNumberOfMovesLeft()
{
	NumberOfMovesLeft = 5;
}

void Nation::simulateTurn(Game_System *Game)
{
	TileAttacking = -1;
	UnitsSkippingThisTurn.clear();
	UnitsSkippingThisTurn.resize(0);
	if (AIState == CaptureNearestCities)
		this->captureNearestCitiesState(Game);
}

void Nation::captureNearestCitiesState(Game_System *Game)
{
	int UnitIndex = -1, GoalIndex = -1, NumOfMoves = NumberOfMovesLeft, MovesMade = 0;

	if (GlobalAppFunctions.Rand.getRandomInt(0, 100) > 50)
	{
		for (int i = 0; i < NumOfMoves; i++)
		{
			this->getUnitsToAttack(Game, UnitIndex, TileAttacking);
			if (Game->Board.isValidTile(TileAttacking))
			{
				if (this->isValidArmy(UnitIndex))
					this->aiMoveUnit(Game, UnitIndex, TileAttacking);
			}	
		}
	}
	
	NumOfMoves = NumberOfMovesLeft;
	for (int i = 0; i < NumOfMoves; i++)
	{
		this->determineNearestCity(Game, UnitIndex, GoalIndex, true);
		if (this->isValidArmy(UnitIndex))
		{
			this->aiMoveUnit(Game, UnitIndex, GoalIndex);
			MovesMade++;
		}
	}
	while (MovesMade < 5)
	{
		this->determineNearestCity(Game, UnitIndex, GoalIndex, false);
		if (this->isValidArmy(UnitIndex))
		{
			this->aiMoveUnit(Game, UnitIndex, GoalIndex);	
		}		
		MovesMade++;
	}
}

void Nation::setPlayerIndex(int Value)
{
	PlayerIndex = Value;
}

void Nation::getUnitsToAttack(Game_System *Game, int &UnitIndex, int &GoalIndex)
{
	if (Game->Board.isValidTile(GoalIndex) != true)
	{
		for (unsigned int i = 0; i < Units.size(); i++)
		{
			if (this->isValidArmy(i))
			{
				int TileIndex = Units[i].getTile();
				if (Game->Board.isValidTile(TileIndex))
				{
					int NeighborHexes[6];
					Game->Board.getHexNeighbors(TileIndex, NeighborHexes);

					for (int j = 0; j < 6; j++)
					{
						if (this->canWeWinAttack(Game, NeighborHexes[j]))
						{
							UnitIndex = i;
							GoalIndex = NeighborHexes[j];
							TileAttacking = NeighborHexes[j];
							return;
						}
					}
				}
			}
		}
	}

	else
	{
		int EnemyOwner = Game->Board.Tiles[GoalIndex].getOwner();
		if (EnemyOwner != PlayerIndex)
		{
			int NeighborHexes[6];
			Game->Board.getHexNeighbors(GoalIndex, NeighborHexes);
			for (int i = 0; i < 6; i++)
			{
				if (Game->Board.isValidTile(NeighborHexes[i]))
				{
					UnitIndex = Game->Board.Tiles[NeighborHexes[i]].getArmyIndex();
					if (!this->isValidArmy(UnitIndex))
					{
						GoalIndex = -1;
						UnitIndex = -1;
						return;
					}

					else
						UnitIndex = UnitIndex;
				}
			}
		}

		//else victory!  we've conquered the tile
		else
			GoalIndex = -1;
	}
}

bool Nation::canWeWinAttack(Game_System *Game, int TileToAttack)
{
	if (Game->Board.isValidTile(TileToAttack))
	{
		int EnemyOwner = Game->Board.Tiles[TileToAttack].getOwner();
		if (EnemyOwner != -1)
		{
			if (EnemyOwner != PlayerIndex)
			{
				int EnemyArmyIndex = Game->Board.Tiles[TileToAttack].getArmyIndex();
				if (Game->Players[EnemyOwner].isValidArmy(EnemyArmyIndex))
				{
					int EnemyArmySize = Game->Players[EnemyOwner].Units[EnemyArmyIndex].getArmySize() + Game->Players[EnemyOwner].Units[EnemyArmyIndex].getExperience();
					int PlayerArmySize = 0;

					int NeighborHexes[6];
					Game->Board.getHexNeighbors(TileToAttack, NeighborHexes);
					for (int i = 0; i < 6; i++)
					{
						if (Game->Board.isValidTile(NeighborHexes[i]))
						{
							if (Game->Board.Tiles[NeighborHexes[i]].getOwner() == PlayerIndex)
							{
								int ArmyIndex = Game->Board.Tiles[NeighborHexes[i]].getArmyIndex();
								if (this->isValidArmy(ArmyIndex))
								{
									PlayerArmySize += Units[ArmyIndex].getArmySize() + Units[ArmyIndex].getExperience();
								}
							}
						}

						if (PlayerArmySize > EnemyArmySize)
							return true;
					}
				}
			}
		}
	}

	return false; 
}


void Nation::determineNearestCity(Game_System *Game, int &UnitIndex, int &CityIndex, bool LookForFullArmies)
{	
	UnitIndex = -1;
	CityIndex = -1;

	int ShortestDistance = -1;
	for (unsigned int i = 0; i < Units.size(); i++)
	{
		if (Units[i].getIsUnitActive())
		{
			if (Units[i].getDidUnitMove() != true)
			{
				if (areWeSkippingUnit(i) != true)
				{
					bool ContinueWithThisUnit = false;
					if (!LookForFullArmies)
					{
						ContinueWithThisUnit = true;
					}

					else
					{
						if (Units[i].getArmySize() == MAX_ARMY_SIZE)
							ContinueWithThisUnit = true;
					}

					if (ContinueWithThisUnit)
					{
						int UnitTile = Units[i].getTile();
						for (unsigned int j = 0; j < Game->Board.CityTiles.size(); j++)
						{
							if (Game->Board.Tiles[Game->Board.CityTiles[j]].getOwner() != PlayerIndex)
							{
								if (UnitIndex == -1)
								{						
									UnitIndex = i;
									CityIndex = Game->Board.CityTiles[j];
									ShortestDistance = Game->Pathfinder.getTurnsToReachGoal(UnitTile, Game->Board.CityTiles[j]);
									if (ShortestDistance == 0)
									{
										UnitIndex = -1;
										CityIndex = -1;
										ShortestDistance = -1;
									}
								}

								else 
								{
									int r = Game->Pathfinder.getTurnsToReachGoal(UnitTile, Game->Board.CityTiles[j]);
									if (r != 0)
									{
										if (r < ShortestDistance)
										{
											UnitIndex = i;
											CityIndex = Game->Board.CityTiles[j];
											ShortestDistance = r;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void Nation::aiMoveUnit(Game_System *Game, int UnitIndex, int GoalTile)
{
	if (UnitIndex < Units.size())
		if (UnitIndex >= 0)
			if (Game->Board.isValidTile(GoalTile))
			{
				int NextTile = Game->Pathfinder.findNextTileToMoveTo(Units[UnitIndex].getTile(), GoalTile);

				if (NextTile != GoalTile)
					NextTile = Game->Pathfinder.findNextTileToMoveTo(NextTile, GoalTile);	

				bool Move = true;
				if (Game->Board.Tiles[NextTile].getOwner() == PlayerIndex)
					if (this->isValidArmy(Game->Board.Tiles[NextTile].getArmyIndex()))
						if (this->Units[Game->Board.Tiles[NextTile].getArmyIndex()].getArmySize() == MAX_ARMY_SIZE)
						{
							Move = false;
							this->UnitsSkippingThisTurn.push_back(UnitIndex);
						}

				if (Move == true)
				{
					if (this->areWeSkippingUnit(UnitIndex) == true)
						Move = false;
					if (Move)
						Game->moveArmy(PlayerIndex, UnitIndex, NextTile);
				}
			}
}

bool Nation::isValidArmy(int Index)
{
	if (Index >= 0)
		if (Index < Units.size())
			if (Units[Index].getIsUnitActive())
				return true;
	return false;
}

bool Nation::areWeSkippingUnit(int Index)
{
	for (int i = 0; i < UnitsSkippingThisTurn.size(); i++)
		if (UnitsSkippingThisTurn[i] == Index)
		{
			return true;
		}

	return false;
}