#ifndef UNIT_H
#define UNIT_H

#include <SFML/Graphics.hpp>

class Unit
{
private:
	int TileIndex, ArmySize, Experience;
	bool DidUnitMove, IsUnitActive;
	sf::Vector2f Position;

public:
	Unit();
	void resetUnit();
		
	void addArmySize(int Value);
	void setArmySize(int Value);
	int getArmySize();

	void addExperience(int Value);
	void setExperience(int Value);
	int getExperience();

	void setDidUnitMove(bool Value);
	bool getDidUnitMove();

	void setIsUnitActive(bool Value);
	bool getIsUnitActive();

	void setPosition(sf::Vector2f Value);
	sf::Vector2f getPosition();

	void setTile(int Value);
	int getTile();
};

#endif 