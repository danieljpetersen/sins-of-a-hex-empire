#include "Board.h"
#include "Constants.h"
#include "Map Generation.h"
#include "Application.h"
#include "Game.h"

Board_Component::Board_Component()
{
	Tiles.resize(NUMBER_OF_TILES);
	CityTiles.resize(0);
}

void Board_Component::generateMap(Game_System *Game)	
{	
	Map_Generation Generator(this);
	Generator.generateMap(GlobalAppFunctions.Rand.getRandomInt(0, 100000));
}

bool Board_Component::isValidTile(int Index)
{
	if (Index < 0)
		return false;

	if (Index >= NUMBER_OF_TILES)
		return false;

	return true;
}

int Board_Component::getTileNeighbor(int CenterTile, int WhichNeighbor)
{
	int HexToTop = -1;
	int HexToBottom = -1;
	
	int HexToTopRight = -1;
	int HexToBottomRight = -1;
	
	int HexToTopLeft = -1;
	int HexToBottomLeft = -1;
	
	if (Tiles[CenterTile].getColumn() % 2 == 0)
	{
		HexToTop = CenterTile - 1;
		HexToBottom = CenterTile + 1;
	
		HexToTopRight = CenterTile + NUMBER_OF_TILES_VERTICAL;
		HexToTopLeft = CenterTile - NUMBER_OF_TILES_VERTICAL;

		HexToBottomRight = CenterTile + (NUMBER_OF_TILES_VERTICAL + 1);
		HexToBottomLeft = CenterTile - (NUMBER_OF_TILES_VERTICAL - 1);
	}
	
	else if (Tiles[CenterTile].getColumn() % 2 != 0)
	{
		HexToTop = CenterTile - 1;
		HexToBottom = CenterTile + 1;
	
		HexToTopRight = CenterTile + (NUMBER_OF_TILES_VERTICAL - 1);	
		HexToTopLeft = CenterTile - (NUMBER_OF_TILES_VERTICAL + 1);

		HexToBottomRight = CenterTile + NUMBER_OF_TILES_VERTICAL;
		HexToBottomLeft = CenterTile - NUMBER_OF_TILES_VERTICAL;
	}
	
	if (WhichNeighbor == HEX_TO_TOP)
	{
		if ((HexToTop < 0) || (Tiles[HexToTop].getRow() == NUMBER_OF_TILES_VERTICAL))
			return -1;
		else
			return HexToTop;
	}

	if (WhichNeighbor == HEX_TO_TOP_RIGHT)
	{
		if ((HexToTopRight >= NUMBER_OF_TILES) || ((Tiles[HexToTopRight].getRow() == NUMBER_OF_TILES_VERTICAL) && (Tiles[CenterTile].getRow() == 1)))
			return -1;
		else
			return HexToTopRight;
	}

	if (WhichNeighbor == HEX_TO_BOTTOM_RIGHT)
	{
		if ((HexToBottomRight >= NUMBER_OF_TILES) || ((Tiles[HexToBottomRight].getRow() == 1) && (Tiles[CenterTile].getRow() == NUMBER_OF_TILES_VERTICAL)))
			return -1;
		else
			return HexToBottomRight;		
	}

	if (WhichNeighbor == HEX_TO_BOTTOM)
	{
		if ((HexToBottom >= NUMBER_OF_TILES) || (Tiles[HexToBottom].getRow() == 1))
			return -1;
		else
			return HexToBottom;
	}

	if (WhichNeighbor == HEX_TO_BOTTOM_LEFT)
	{
		if ((HexToBottomLeft < 0) || ((Tiles[HexToBottomLeft].getRow() == 1) && (Tiles[CenterTile].getRow() == NUMBER_OF_TILES_VERTICAL)))
			return -1;
		else			
			return HexToBottomLeft;
	}
	
	if (WhichNeighbor == HEX_TO_TOP_LEFT)
	{
		if ((HexToTopLeft < 0) || ((Tiles[HexToTopLeft].getRow() == NUMBER_OF_TILES_VERTICAL) && (Tiles[CenterTile].getRow() == 1)))
			return -1;
		else
			return HexToTopLeft;
	}

	return 0;
}

bool Board_Component::isPointInsideTile(int x, int y, int Index)
{
	sf::Vector2f HexPoint1;
	sf::Vector2f HexPoint2;
	sf::Vector2f HexPoint3;
	sf::Vector2f HexPoint4;
	sf::Vector2f HexPoint5;
	sf::Vector2f HexPoint6;
	
	this->getHexPoints(Index, HexPoint1, HexPoint2, HexPoint3, HexPoint4, HexPoint5, HexPoint6);

	sf::Vector2f LinePoint1((float)x, (float)y);
	sf::Vector2f LinePoint2(float(x + (float)GlobalAppFunctions.getWindowSize().x), (float)y + (float)TILE_WIDTH);	
	
	int Intersections = 0;
	if (GlobalAppFunctions.DoLineSegmentsIntersect(LinePoint1, LinePoint2, HexPoint1, HexPoint2) == true)
		Intersections++;
	if (GlobalAppFunctions.DoLineSegmentsIntersect(LinePoint1, LinePoint2, HexPoint2, HexPoint3) == true)
		Intersections++;	
	if (GlobalAppFunctions.DoLineSegmentsIntersect(LinePoint1, LinePoint2, HexPoint3, HexPoint4) == true)
		Intersections++;
	if (GlobalAppFunctions.DoLineSegmentsIntersect(LinePoint1, LinePoint2, HexPoint4, HexPoint5) == true)
		Intersections++;
	if (GlobalAppFunctions.DoLineSegmentsIntersect(LinePoint1, LinePoint2, HexPoint5, HexPoint6) == true)
		Intersections++;
	if (GlobalAppFunctions.DoLineSegmentsIntersect(LinePoint1, LinePoint2, HexPoint6, HexPoint1) == true)
		Intersections++;

	if ((Intersections == 1) || (Intersections == 3))
		return true;
	else
		return false;
}

void Board_Component::getHexPoints(int Index, sf::Vector2f &Point1, sf::Vector2f &Point2, sf::Vector2f &Point3, sf::Vector2f &Point4, sf::Vector2f &Point5, sf::Vector2f &Point6)
{
	if (isValidTile(Index))
	{
		float xBoundingRegion = Tiles[Index].getPosition().x;
		float yBoundingRegion = Tiles[Index].getPosition().y;

		Point1.x = xBoundingRegion + 1;
		Point1.y = 1 + yBoundingRegion + TILE_HEIGHT / 2;

		Point2.x = xBoundingRegion + 13;
		Point2.y = yBoundingRegion + 1;

		Point3.x = xBoundingRegion + 39;
		Point3.y = Point2.y;

		Point4.x = xBoundingRegion + TILE_WIDTH - 1;
		Point4.y = Point1.y;

		Point5.x = Point3.x;
		Point5.y = yBoundingRegion + TILE_HEIGHT - 1;

		Point6.x = Point2.x;
		Point6.y = Point5.y;
	}
}

void Board_Component::getHexNeighbors(int Center, int (&Array)[6])
{
	if (isValidTile(Center) == true)
	{
		Array[0] = this->getTileNeighbor(Center, 1);
		Array[1] = this->getTileNeighbor(Center, 2);
		Array[2] = this->getTileNeighbor(Center, 3);
		Array[3] = this->getTileNeighbor(Center, 4);
		Array[4] = this->getTileNeighbor(Center, 5);
		Array[5] = this->getTileNeighbor(Center, 6);
	}
	else
	{
		Array[0] = -1;
		Array[1] = -1;
		Array[2] = -1;
		Array[3] = -1;
		Array[4] = -1;
		Array[5] = -1;
	}
}

void Board_Component::cheapHackEnsureArmyOnTileIsCorrect(Game_System *Game)
{
	for (int i = 0; i < NUMBER_OF_TILES; i++)
		Tiles[i].setArmyIndex(-1);

	for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
	{
		for (int j = 0; j < Game->Players[i].Units.size(); j++)
		{
			if (Game->Players[i].Units[j].getIsUnitActive())
				Tiles[Game->Players[i].Units[j].getTile()].setArmyIndex(j);

		}
	}
}