#ifndef BOARD_H
#define BOARD_H
 
#include <SFML/System.hpp>

#include "Tile.h"
#include <vector>

class Game_System;

class Board_Component
{
private:
	void getHexPoints(int Index, sf::Vector2f &Point1, sf::Vector2f &Point2, sf::Vector2f &Point3, sf::Vector2f &Point4, sf::Vector2f &Point5, sf::Vector2f &Point6);

	void generateMap(Game_System *Game);
public:
	Board_Component();

	static const int NUMBER_OF_TILES_HORIZONTAL = 20;
	static const int NUMBER_OF_TILES_VERTICAL = 11;
	static const int NUMBER_OF_TILES = NUMBER_OF_TILES_HORIZONTAL * NUMBER_OF_TILES_VERTICAL;
	static const int LAST_TILE_FIRST_ROW = NUMBER_OF_TILES_VERTICAL - 1;
	static const int LAST_TILE_ON_MAP = NUMBER_OF_TILES - 1;
	
	std::vector<Tile> Tiles;
	
	void initTileProperties();

	bool isValidTile(int Index);
	
	bool isPointInsideTile(int x, int y, int Index);

	int getTileNeighbor(int CenterTile, int WhichNeighbor);
	
	void getHexNeighbors(int Center, int (&Array)[6]);

	std::vector<int> CityTiles;

	void cheapHackEnsureArmyOnTileIsCorrect(Game_System *Game);

	friend class Game_System;
};
 
#endif 