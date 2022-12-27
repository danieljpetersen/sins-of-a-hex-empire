#include "Game.h"
#include "Application.h"
#include "Constants.h"

Game_System::Game_System(SFML_Wrapper::CoreApplication *TheApp)
{	
	App = TheApp;

	HumanNationIndex = 0;

	Players.resize(NUMBER_OF_PLAYERS);

	for (int i = 0; i < 18; i++)
		TilesInRange[i] = -1;

	TurnNumber = 1;	
	ActivePlayer = 0;
	SelectedTile = -1;

	Pathfinder.setBoardPointer(&Board);
	
	this->generateMap();
}

void Game_System::endTurn()
{
	ActivePlayer++;
	if (ActivePlayer > 3)
		ActivePlayer = 0;
	for (unsigned int i = 0; i < Players.size(); i++)
	{
		unsigned int NumOfArmies = Players[ActivePlayer].Units.size();
		for (unsigned int j = 0; j < NumOfArmies; j++)
		{
			Players[ActivePlayer].Units[j].setDidUnitMove(false);
		}
		
		//if (Players[ActivePlayer].Units.size() == 0)
			this->spawnArmies(ActivePlayer);
		Players[ActivePlayer].resetNumberOfMovesLeft();
		if (Players[ActivePlayer].getAIControlled())
			Players[ActivePlayer].simulateTurn(this);
	
		

		ActivePlayer++;
		if (ActivePlayer > 3)
			ActivePlayer = 0;
	}
	GlobalAppFunctions.UpdateTiles = true;

	TurnNumber++;
}

bool Game_System::moveArmy(int PlayerIndex, int UnitIndex, int NewTile)
{
	if (this->Board.isValidTile(NewTile) == true)
	{
		if (PlayerIndex != -1)
		{
			if (Players[PlayerIndex].isValidArmy(UnitIndex))
			{
				int TileType = this->Board.Tiles[NewTile].getType();
				if (TileType != WATER)
				{
					int NewTileOwner = Board.Tiles[NewTile].getOwner(); 
					int NewTileArmyIndex = Board.Tiles[NewTile].getArmyIndex();
					bool InitiateAttack = false;
					if (NewTileOwner != PlayerIndex)
						if (NewTileArmyIndex != -1)
							InitiateAttack = true;

					if (!InitiateAttack)
					{
						bool DidEntireArmyTransfer = false;
						int OldTile = Players[PlayerIndex].Units[UnitIndex].getTile();
						bool DidWeMove = this->addArmyToTile(NewTile, PlayerIndex, UnitIndex, DidEntireArmyTransfer);
						this->setSelectedTile(-1);
						if (DidWeMove)
						{
							this->Players[PlayerIndex].Units[UnitIndex].setDidUnitMove(true);
							if (DidEntireArmyTransfer != true)
							{
								Players[PlayerIndex].Units[Board.Tiles[NewTile].getArmyIndex()].setDidUnitMove(true);
							}
						}
						expandBorders(NewTile, PlayerIndex);
						Players[PlayerIndex].decrementNumberOfMovesLeft();

						return true;
					}

					else
					{
						bool AttackerWon = initiateAttack(PlayerIndex, UnitIndex, NewTileOwner, NewTileArmyIndex);
						if (AttackerWon)
							Players[PlayerIndex].decrementNumberOfMovesLeft();
						else
						{
							Players[PlayerIndex].Units[UnitIndex].resetUnit();
							Players[PlayerIndex].Units[UnitIndex].setIsUnitActive(false);							
						}

						return true;
					}
				}
			}
		}
	}			

	Board.cheapHackEnsureArmyOnTileIsCorrect(this);
	return false;
}

bool Game_System::initiateAttack(int AttackingPlayer, int AttackingArmy, int DefendingPlayer, int DefendingArmy)
{
	bool ValidArmies = true;
	if (!Players[AttackingPlayer].isValidArmy(AttackingArmy))
		ValidArmies = false;
	if (!Players[DefendingPlayer].isValidArmy(DefendingArmy))
		ValidArmies = false;

	if (ValidArmies)
	{
		bool DidAttackerWin = true;
		int AttackerSize = Players[AttackingPlayer].Units[AttackingArmy].getArmySize();
		int AttackerExperience = Players[AttackingPlayer].Units[AttackingArmy].getExperience();

		int DefenderSize = Players[DefendingPlayer].Units[DefendingArmy].getArmySize();
		int DefenderExperience = Players[DefendingPlayer].Units[DefendingArmy].getExperience();

		bool quit = false;
		while (quit == false)
		{
			if ((AttackerSize == 0) || (DefenderSize == 0))
			{
				quit = true;
				break;
			}
			if (AttackerExperience > 0)
				AttackerExperience--;
			else
				AttackerSize--;

			if (DefenderExperience > 0)
				DefenderExperience--;
			else
				DefenderSize--;
		}

		int AttackerTile = Players[AttackingPlayer].Units[AttackingArmy].getTile();
		int DefenderTile = Players[DefendingPlayer].Units[DefendingArmy].getTile();

		Players[AttackingPlayer].Units[AttackingArmy].setExperience(AttackerExperience);
		Players[AttackingPlayer].Units[AttackingArmy].setArmySize(AttackerSize);

		Players[DefendingPlayer].Units[DefendingArmy].setExperience(DefenderExperience);
		Players[DefendingPlayer].Units[DefendingArmy].setArmySize(DefenderSize);

		//if the defender won
		if (DefenderSize > 0)
		{
			this->removeArmyFromTile(AttackerTile);	
			return false;
		}

		else
		{  
			if ((DefenderTile == TOP_LEFT_CAPITAL) && (DefendingPlayer == 0))
				killPlayerOff(DefendingPlayer, AttackingPlayer);
			if ((DefenderTile == TOP_RIGHT_CAPITAL) && (DefendingPlayer == 2))
				killPlayerOff(DefendingPlayer, AttackingPlayer);
			if ((DefenderTile == BOTTOM_LEFT_CAPITAL) && (DefendingPlayer == 1))
				killPlayerOff(DefendingPlayer, AttackingPlayer);
			if ((DefenderTile == BOTTOM_RIGHT_CAPITAL) && (DefendingPlayer == 3))
				killPlayerOff(DefendingPlayer, AttackingPlayer);
			this->removeArmyFromTile(DefenderTile);			
			this->moveArmy(AttackingPlayer, AttackingArmy, DefenderTile);
			
			return true;
		}
	}

	return false;
}

void Game_System::removeArmyFromTile(int TileIndex)
{
	Board.Tiles[TileIndex].setArmyIndex(-1);
}

bool Game_System::addArmyToTile(int TileToAddTo, int PlayerIndex, int ArmyIndex, bool &DidEntireArmyTransfer)
{
	bool DidWeMove = true;
	int ArmyOnTileToAddTo = Board.Tiles[TileToAddTo].getArmyIndex();
	int OriginalTileForUnitGettingAdded = Players[PlayerIndex].Units[ArmyIndex].getTile();

	//if there is no army on the tile that we're adding to
	if (ArmyOnTileToAddTo == -1)
	{
		Board.Tiles[TileToAddTo].setArmyIndex(ArmyIndex);
		Players[PlayerIndex].Units[ArmyIndex].setPosition(Board.Tiles[TileToAddTo].getPosition());
		Players[PlayerIndex].Units[ArmyIndex].setTile(TileToAddTo);

		//if the army we're adding exists on another tile
		if (OriginalTileForUnitGettingAdded != -1)
			this->removeArmyFromTile(OriginalTileForUnitGettingAdded);

		DidEntireArmyTransfer = true;
	}
	
	//if there is already an army on the tile that we're adding to
	else
	{
		int TotalArmySize = Players[PlayerIndex].Units[ArmyOnTileToAddTo].getArmySize() + Players[PlayerIndex].Units[ArmyIndex].getArmySize();
		int TotalExperience = Players[PlayerIndex].Units[ArmyOnTileToAddTo].getExperience() + Players[PlayerIndex].Units[ArmyIndex].getExperience();

		//situation where we do not move
		if ((Players[PlayerIndex].Units[ArmyOnTileToAddTo].getArmySize() == MAX_ARMY_SIZE) && ((Players[PlayerIndex].Units[ArmyOnTileToAddTo].getExperience() == MAX_EXPERIENCE) || (Players[PlayerIndex].Units[ArmyIndex].getExperience() == 0)))
			DidWeMove = false;
		
		//we leave both units in tact and move what we can.  any extra experience gets wasted
		else if (TotalArmySize > MAX_ARMY_SIZE)
		{
			int MovingUnitArmySize = Players[PlayerIndex].Units[ArmyIndex].getArmySize();
			int NewTileArmySize = Players[PlayerIndex].Units[ArmyOnTileToAddTo].getArmySize();

			while ((NewTileArmySize != MAX_ARMY_SIZE) && (MovingUnitArmySize > 0))
			{
				MovingUnitArmySize--;
				NewTileArmySize++;
				Players[PlayerIndex].Units[ArmyIndex].setArmySize(Players[PlayerIndex].Units[ArmyIndex].getArmySize() - 1);
				Players[PlayerIndex].Units[ArmyOnTileToAddTo].addArmySize(1);
			}
			
			int MovingUnitExperienceSize = Players[PlayerIndex].Units[ArmyIndex].getExperience();
			int NewTileExperienceSize = Players[PlayerIndex].Units[ArmyOnTileToAddTo].getExperience();
			while ((NewTileExperienceSize != MAX_EXPERIENCE) && (MovingUnitExperienceSize > 0))
			{
				MovingUnitExperienceSize--;
				NewTileExperienceSize++;
				Players[PlayerIndex].Units[ArmyIndex].setExperience(Players[PlayerIndex].Units[ArmyIndex].getExperience() - 1);
				Players[PlayerIndex].Units[ArmyOnTileToAddTo].addExperience(1);
				
			}

			if (Players[PlayerIndex].Units[ArmyIndex].getArmySize() == 0)
			{
				DidEntireArmyTransfer = true;				
				if (OriginalTileForUnitGettingAdded != -1)
					this->removeArmyFromTile(OriginalTileForUnitGettingAdded);
			}
			else
				DidEntireArmyTransfer = false;
		}

		//we move the entire thing
		else
		{
			this->removeArmyFromTile(OriginalTileForUnitGettingAdded);
			Players[PlayerIndex].Units[ArmyOnTileToAddTo].addArmySize(Players[PlayerIndex].Units[ArmyIndex].getArmySize());
			Players[PlayerIndex].Units[ArmyOnTileToAddTo].addExperience(Players[PlayerIndex].Units[ArmyIndex].getExperience());
			Players[PlayerIndex].Units[ArmyOnTileToAddTo].setTile(TileToAddTo);

			Players[PlayerIndex].Units[ArmyIndex].setIsUnitActive(false);

			DidEntireArmyTransfer = true;
		}
	}

	return DidWeMove;
}


int Game_System::getHumanNationIndex()
{
	return HumanNationIndex;
}

void Game_System::setHumanNation(int Value)
{
	if (Value >= 0)
		if (Value <= 3)
		{
			HumanNationIndex = Value;
			Players[HumanNationIndex].setAIControllerd(false);
			ActivePlayer = Value;
		}
}

void Game_System::spawnArmies(int PlayerIndex)
{
	for (int i = 0; i < Board.NUMBER_OF_TILES; i++)
	{
		int NumToAdd = 0;
		if (Board.Tiles[i].getType() == CITY)
			NumToAdd = 1;
		if (Board.Tiles[i].getType() == CAPITAL)
			NumToAdd = 2;
		
		if (NumToAdd != 0)
		{
			int Owner = Board.Tiles[i].getOwner();
			if (Owner == PlayerIndex)
			{	
				if (i == BOTTOM_RIGHT_CAPITAL)
					int r = 0;

				//if there's an army already there
				int ArmyIndex = Board.Tiles[i].getArmyIndex();
				if (ArmyIndex != -1)
				{
					int ArmySize = Players[Owner].Units[ArmyIndex].getArmySize();
					Players[Owner].Units[ArmyIndex].setArmySize(ArmySize + NumToAdd);
				}

				//if there's no army
				else
				{
					//find any armies on the index which aren't being used
					ArmyIndex = this->findFreeArmyToAdd(Owner);

					//if none were found
					if (ArmyIndex == -1)
					{
						Players[Owner].Units.resize(Players[Owner].Units.size() + 1);
						Players[Owner].Units.back().resetUnit();
						Players[Owner].Units.back().setArmySize(NumToAdd);
						bool DidEntireArmyTransfer = false;
						this->addArmyToTile(i, Owner, Players[Owner].Units.size() - 1, DidEntireArmyTransfer);
					}

					else
					{
						int ArmySize = Players[Owner].Units[ArmyIndex].getArmySize();
						Players[Owner].Units[ArmyIndex].setArmySize(ArmySize + NumToAdd);
						bool DidEntireArmyTransfer = false;
						this->addArmyToTile(i, Owner, ArmyIndex, DidEntireArmyTransfer);
					}
				}
			}
		}
	}
}

int Game_System::findFreeArmyToAdd(int Player)
{
	for (unsigned int i = 0; i < Players[Player].Units.size(); i++)
	{
		if (!Players[Player].Units[i].getIsUnitActive())
		{
			Players[Player].Units[i].resetUnit();
			return i;
		}
	}

	return -1;
}

void Game_System::addInitialArmies()
{
	for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
		Players[i].Units.resize(0);
	
	ActivePlayer = HumanNationIndex;
	
	this->spawnArmies(0);
	this->spawnArmies(1);
	this->spawnArmies(2);
	this->spawnArmies(3);
	
	for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
	{
		int TileToAddArmies = -1;
				
		if (i == 0)
			TileToAddArmies = TOP_LEFT_CAPITAL;
		if (i == 1)
			TileToAddArmies = BOTTOM_LEFT_CAPITAL;

		if (i == 2)
			TileToAddArmies = TOP_RIGHT_CAPITAL;
		
		if (i == 3)
			TileToAddArmies = BOTTOM_RIGHT_CAPITAL;
		

		Players[i].Units[0].setArmySize(5);
		Players[i].Units[0].setExperience(5);
	}

	GlobalAppFunctions.ProgramState = Normal;
}

void Game_System::setActivePlayer(int Value)
{
	if (Value >= 0)
		if (Value <= 3)
			ActivePlayer = Value;
}

int Game_System::getActivePlayer()
{
	return ActivePlayer;
}

void Game_System::setSelectedTile(int Value)
{
	SelectedTile = Value;
	if (Board.isValidTile(Value))
		GlobalAppFunctions.UpdateTiles = true;
}

int Game_System::getSelectedTile()
{
	return SelectedTile;
}

int Game_System::getTilesInMovementRange(int Index)
{
	return TilesInRange[Index];
}

void Game_System::determineTilesInRange()
{
	//put a visual outline in any hex that the currently selected unit is able to travel to	
	for (int i = 0; i < 18; i++)
	{
		TilesInRange[i] = -1;
	}
	int TilesInRangeIndex = 0;

	//loop through immediate tile neighbors which aren't water
		//loop through their neighbors which aren't water, adding any tile not already in the list to the list
	for (int i = 1; i <= NUMBER_OF_ADJACENT_TILES; i++)
	{
		int SelectedNeighbor = Board.getTileNeighbor(SelectedTile, i);

		if (SelectedNeighbor != -1)
			if (this->isTileInTilesInRange(SelectedNeighbor) != true)
				if (Board.Tiles[SelectedNeighbor].getType() != WATER)
					TilesInRange[TilesInRangeIndex++] = SelectedNeighbor;

		if (SelectedNeighbor != -1)
			if (Board.Tiles[SelectedNeighbor].getType() != WATER)	
			{
				for (int j = 1; j <= NUMBER_OF_ADJACENT_TILES; j++)
				{
					int OuterNeighbor = Board.getTileNeighbor(SelectedNeighbor, j);
					if (OuterNeighbor != -1)
						if (OuterNeighbor != SelectedTile)
							if (Board.Tiles[OuterNeighbor].getType() != WATER)
								if (this->isTileInTilesInRange(OuterNeighbor) != true)
								{
									TilesInRange[TilesInRangeIndex++] = OuterNeighbor;
								}
				}
			}
	}
}

bool Game_System::isHexInRange(int TileClicked)
{
	int AdjacentTileCounter = 1;
	int AdjacentTile = -1;

	int OuterCounter = 1;
	int GettingNeighborsOfThisTile = TileClicked;

	//if the hex we clicked on is within two hexes of the hex selected, return true
	while (OuterCounter <= 7)
	{
		while (AdjacentTileCounter <= NUMBER_OF_ADJACENT_TILES)
		{
			AdjacentTile = Board.getTileNeighbor(GettingNeighborsOfThisTile, AdjacentTileCounter);

			if (AdjacentTile == SelectedTile)
				return true;

			AdjacentTileCounter++;
		}
		GettingNeighborsOfThisTile = Board.getTileNeighbor(TileClicked, OuterCounter);
		OuterCounter++;
		
		//if the unit isn't in water
		if (Board.Tiles[SelectedTile].getType() != WATER)
		{
			//prevent us from walking over water and from walking over units			
			int ArmyIndex = Board.Tiles[GettingNeighborsOfThisTile].getArmyIndex();
			int PlayerIndex = Board.Tiles[GettingNeighborsOfThisTile].getOwner();
			int ArmySizeOnNeighborTile = 0;
			if (ArmyIndex != -1)
				if (PlayerIndex != -1)
				{
					while (((Board.Tiles[GettingNeighborsOfThisTile].getType() == WATER) || (ArmySizeOnNeighborTile != 0)) && (OuterCounter <= 7))
					{
						GettingNeighborsOfThisTile = Board.getTileNeighbor(TileClicked, OuterCounter);
						OuterCounter++;
					}
				}
		}

		AdjacentTileCounter = 1;
	}
	return false;
}

void Game_System::resetTilesInMovementRange()
{
	for (int i = 0; i < 18; i++)
		TilesInRange[i] = -1;
}

void Game_System::generateMap()
{
	Players.resize(0);
	Players.resize(4);

	for (int i = 0; i < 4; i++)
		Players[i].setPlayerIndex(i);
	
	TurnNumber = 1;	
	ActivePlayer = 0;
	SelectedTile = -1;

	std::list<int> Path;
	bool ValidBoard = false;
	Board.generateMap(this);
	Pathfinder.analyseBoard();
	while (!ValidBoard)
	{
		if (Pathfinder.checkIfValidAndFinalizeBoard() != true)
		{
			Board.generateMap(this);
			Pathfinder.analyseBoard();
		}
		else
			ValidBoard = true;
	}

	this->resetTilesInMovementRange();
	SelectedTile = -1;
	GlobalAppFunctions.ProgramState = ChooseNation;
	HumanNationIndex = -1;
}

void Game_System::expandBorders(int HexMovedTo, int PlayerIndex)
{
	int AdjacentTileCounter = 1;
	int AdjacentTile = -1;

	int ArmyIndex = Board.Tiles[HexMovedTo].getArmyIndex();

	//if we land on a plce that's not ours, add experience and set the tile color to ours
	if (Board.Tiles[HexMovedTo].getOwner() != PlayerIndex)
	{
		Board.Tiles[HexMovedTo].setOwner(PlayerIndex);
		Players[PlayerIndex].Units[ArmyIndex].addExperience(1);
	}

	//set the border of every appropriate adjacent tile to the color of whoever moved
	while (AdjacentTileCounter <= NUMBER_OF_ADJACENT_TILES)
	{
		AdjacentTile = Board.getTileNeighbor(HexMovedTo, AdjacentTileCounter);
		if (AdjacentTile != -1)
		{
			int AdjacentUnitIndex = Board.Tiles[AdjacentTile].getArmyIndex();
	
			if (AdjacentUnitIndex == -1)
				if (Board.Tiles[AdjacentTile].getType() != CITY)
					if (Board.Tiles[AdjacentTile].getType() != WATER)
						if (Board.Tiles[AdjacentTile].getType() != CAPITAL)
							if (Board.Tiles[AdjacentTile].getOwner() != PlayerIndex)
							{
								Board.Tiles[AdjacentTile].setOwner(PlayerIndex);
								Players[PlayerIndex].Units[ArmyIndex].addExperience(1);
							}
		}
		AdjacentTileCounter++;
	}
}

bool Game_System::isTileInTilesInRange(int TileIndex)
{
	for (int i = 0; i < 18; i++)
	{
		if (TileIndex == getTilesInMovementRange(i))
			return true;
	}

	return false;
}

void Game_System::killPlayerOff(int PlayerToKill, int PlayerGainingTerritories)
{
	for (int i = 0; i < Players[PlayerToKill].Units.size(); i++)
	{
		Players[PlayerToKill].Units[i].resetUnit();
		Players[PlayerToKill].Units[i].setIsUnitActive(false);
	}
	Players[PlayerToKill].Units.resize(0);

	for (int i = 0; i < Board.NUMBER_OF_TILES; i++)
	{
		if (Board.Tiles[i].getOwner() == PlayerToKill)
			Board.Tiles[i].setOwner(PlayerGainingTerritories);
	}
	this->Board.cheapHackEnsureArmyOnTileIsCorrect(this);
}