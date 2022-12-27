#ifndef TILE_H
#define TILE_H
 
#include <SFML/Graphics.hpp>

class Tile
{
private:
	int Column, Row, Type, Owner, ArmyIndex;
	sf::Vector2f Position;
	
	void setPosition(sf::Vector2f NewPosition);	
	void setRow(int Value);
	void setColumn(int Value);

public:
	Tile();
	void init();
	
	int getType();

	void setOwner(int Value);

	int getOwner();

	int getArmyIndex();//for any army currently on the tile

	void setArmyIndex(int Value);

	int getColumn();

	int getRow();

	sf::Vector2f getPosition();

	void setType(int Value);	

	friend class Map_Generation;
};

#endif 