#include "Map Generation.h"
#include "Board.h"
#include "Constants.h"
#include "Application.h"

Map_Generation::Map_Generation(Board_Component *BoardObject)
{
	Board = BoardObject;
}

//==================================

void Map_Generation::initTileProperties()
{
	Board->Tiles.resize(0);
	Board->Tiles.resize(Board->NUMBER_OF_TILES);
	
	int Column = 1, Row = 0;
	int Counter = 0;
	while (Counter < Board->NUMBER_OF_TILES)
	{
		Row++;

		Board->Tiles[Counter].setColumn(Column);
		Board->Tiles[Counter].setRow(Row);

		if (Row == Board->NUMBER_OF_TILES_VERTICAL)
		{
			Column++;
			Row = 0;
		}
		
		Counter++;
	}
	this->centerBoard();
}

//==================================

void Map_Generation::clearBoard()
{
	for (int i = 0; i < Board->NUMBER_OF_TILES; i++)
	{
		Board->Tiles[i].setType(LAND);
		Board->Tiles[i].setOwner(-1);
		Board->Tiles[i].setArmyIndex(-1);
	}
}

//==================================

void Map_Generation::centerBoard()
{	
	//a terrible function, but the code is old and I don't feel like redoing it
	//width is slightly to the left 
	int xBoardStart = 0, yBoardStart = 0;
	const int OddRowWidthIncrease = 26;
	const int OddLastRowWidthIncrease = 11;
	const int EvenRowWidthIncrease = TILE_WIDTH - 1;
	int WidthOfBoard = 0;

	int NumberOfOddRows = 0, NumberOfEvenRows = 0;
	if (Board->NUMBER_OF_TILES_HORIZONTAL % 2 == 0)
	{
		NumberOfOddRows = Board->NUMBER_OF_TILES_HORIZONTAL / 2;
		NumberOfEvenRows = Board->NUMBER_OF_TILES_HORIZONTAL / 2;
	
		WidthOfBoard = OddLastRowWidthIncrease;	
	}
	else
	{
		NumberOfOddRows = Board->NUMBER_OF_TILES_HORIZONTAL / 2;
		NumberOfEvenRows = Board->NUMBER_OF_TILES_HORIZONTAL / 2 + 1;
	}
	
	WidthOfBoard += (NumberOfOddRows * OddRowWidthIncrease) + (NumberOfEvenRows * EvenRowWidthIncrease);
	GlobalAppFunctions.centerPosition(0, (int)GlobalAppFunctions.getCameraSize().x, WidthOfBoard, xBoardStart);
	xBoardStart += 2;//???

	const int HeightOfBoard = (((Board->NUMBER_OF_TILES_VERTICAL) * TILE_HEIGHT) + TILE_HEIGHT / 2) - Board->NUMBER_OF_TILES_VERTICAL + 1;
	GlobalAppFunctions.centerPosition(0, (int)GlobalAppFunctions.getCameraSize().y, HeightOfBoard, yBoardStart);

	const int HALF_HEX_HEIGHT = TILE_HEIGHT / 2;
	int CounterVertical = 0, CounterHorizontal = 0, Index = 0;
	
	while (CounterHorizontal < Board->NUMBER_OF_TILES_HORIZONTAL)
	{
		while (CounterVertical < Board->NUMBER_OF_TILES_VERTICAL)
		{
			float xPosition = float(((37.0f) * CounterHorizontal) + xBoardStart);
			float yPosition = 0;
			if ((CounterHorizontal % 2 == 0) || (CounterHorizontal == 0))
				yPosition = float(((TILE_HEIGHT - 1) * CounterVertical) + yBoardStart);
			else
				yPosition = float((((TILE_HEIGHT - 1) * CounterVertical) + HALF_HEX_HEIGHT) + yBoardStart);
				
			Board->Tiles[Index].setPosition(sf::Vector2f(xPosition, yPosition));

			CounterVertical++;
			Index++;
		}
		CounterVertical = 0;
		CounterHorizontal++;
	}
}

//==================================

void Map_Generation::generateMap(int Seed)
{
	clearBoard();
	initTileProperties();

	generateWater(Seed);
	generateStartingAreas(Seed);
	generateCities(Seed);

	GlobalAppFunctions.UpdateTiles = true;
}

//==================================

void Map_Generation::generateWater(int Seed)
{
	int NumberOfOceans = GlobalAppFunctions.Rand.getRandomInt(4, 12);
	int LocationOfOcean = GlobalAppFunctions.Rand.getRandomInt(0, Board->NUMBER_OF_TILES - 1);

	int TempBoard[Board->NUMBER_OF_TILES] = {0};

	//initialize tempboard 
	for (int i = 0; i < Board->NUMBER_OF_TILES; i++)
	{
		TempBoard[i] = -1;
	}

	//throw down an initial tile of water at random locations
	for (int i = 0; i < NumberOfOceans; i++)
	{
		Board->Tiles[LocationOfOcean].setType(WATER);
		TempBoard[LocationOfOcean] = WATER;
		LocationOfOcean = GlobalAppFunctions.Rand.getRandomInt(0, Board->NUMBER_OF_TILES - 1);
	}

	//expand the initial water tiles into larger bodies of water
	for (int i = 0; i < Board->NUMBER_OF_TILES; i++)
	{
		if (TempBoard[i] == WATER)
		{
			int SizeOfOcean = GlobalAppFunctions.Rand.getRandomInt(1, 35);
			int SizeOfOceanCounter = 0;
			int NextWaterTile = i, LastWaterTile = i;
			
			while (SizeOfOceanCounter < SizeOfOcean)
			{
				Board->Tiles[NextWaterTile].setType(WATER);
				LastWaterTile = NextWaterTile;
				NextWaterTile = -1;
				while (NextWaterTile == -1)
					NextWaterTile = Board->getTileNeighbor(LastWaterTile, GlobalAppFunctions.Rand.getRandomInt(1, 6));
				SizeOfOceanCounter++;
			}
		}
	}
}

//==================================

void Map_Generation::generateStartingAreas(int Seed)
{
	int CapitalPosition = TOP_LEFT_CAPITAL, TileToMakeLand = -1;

	for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
	{
		if (i == 1)
			CapitalPosition = BOTTOM_LEFT_CAPITAL;

		if (i == 2)
			CapitalPosition = TOP_RIGHT_CAPITAL;
		
		if (i == 3)
			CapitalPosition = BOTTOM_RIGHT_CAPITAL;
		
		Board->Tiles[CapitalPosition].setType(CAPITAL);
		Board->Tiles[CapitalPosition].setOwner(i);

		//clear land around capital
		for (int j = HEX_TO_TOP; j < 7; j++)
		{
			TileToMakeLand = Board->getTileNeighbor(CapitalPosition, j);
			if (TileToMakeLand != -1)
			{
				Board->Tiles[TileToMakeLand].setType(LAND);
				Board->Tiles[TileToMakeLand].setOwner(i);
			}
		}
	}
	
}

//==================================

void Map_Generation::generateCities(int Seed)
{
	int NextCityTile = -1, NumberOfCitiesPlaced = 0, NumberOfTries = 0, MaxNumberOfTries=500;
	const int DESIRED_NUMBER_OF_CITIES = 20;
	for (int i = 0; i < DESIRED_NUMBER_OF_CITIES; i++)
	{
		bool FoundCityLocation = false;
		while (!FoundCityLocation)
		{
			//prevent infinite loop on maps with a lot of water
			NextCityTile = GlobalAppFunctions.Rand.getRandomInt(0, Board->NUMBER_OF_TILES - 1);
			NumberOfTries++;
			if (NumberOfTries > MaxNumberOfTries)
				break;

			if (Board->Tiles[NextCityTile].getOwner() == -1)
			{
				if (Board->Tiles[NextCityTile].getType() == LAND)
				{				
					bool NeighborsAreSuitable = true;
					int NumOfNeighborWaterTiles = 0;
					for (int j = 1; j < 7; j++)
					{
						int Neighbor = Board->getTileNeighbor(NextCityTile, j);
						if (Board->isValidTile(Neighbor))
						{
							if ((Board->Tiles[Neighbor].getType() != LAND) && (Board->Tiles[Neighbor].getType() != WATER) || (Board->Tiles[Neighbor].getOwner() != -1))
							{
								NeighborsAreSuitable = false;
								break;
							}

							if (Board->Tiles[Neighbor].getType() == WATER)
								NumOfNeighborWaterTiles++;
						}
					}

					//don't want a city surrounded completely by water
					if (NumOfNeighborWaterTiles == 6)
						NeighborsAreSuitable = false;

					if (NeighborsAreSuitable)
						FoundCityLocation = true;
				}
			}
			if (FoundCityLocation)
			{
				if (Board->isValidTile(NextCityTile))
				{
					Board->Tiles[NextCityTile].setOwner(-1);
					Board->Tiles[NextCityTile].setType(CITY);
					NumberOfCitiesPlaced++;
				}
			}
		}
	}
}

//==================================
