#ifndef MAP_GENERATION_H
#define MAP_GENERATION_H
 
#include <vector>
#include <list>

class Board_Component;
class Game_System;
class Application;

class Map_Generation
{
private:
	Board_Component *Board;

public:
	Map_Generation(Board_Component *BoardObject);

	void initTileProperties();
	void clearBoard();
	void centerBoard();
	void generateMap(int Seed);
	void generateWater(int Seed);
	void generateStartingAreas(int Seed);
	void generateCities(int Seed);
};

#endif 