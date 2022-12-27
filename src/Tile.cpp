#include "Tile.h"
#include "Constants.h"

Tile::Tile()
{
}

//==================================

void Tile::init()
{
	Row = 0;
	Column = 0;

	Owner = -1;
	Type = LAND;
	ArmyIndex = -1;
}

//==================================

int Tile::getType()
{
	return Type;
}

//==================================

void Tile::setType(int Value)
{
	Type = Value;
}

//==================================

void Tile::setRow(int Value)
{
	Row = Value;
}

//==================================

int Tile::getRow()
{
	return Row;
}

//==================================

void Tile::setColumn(int Value)
{
	Column = Value;
}

//==================================

int Tile::getColumn()
{
	return Column;
}

//==================================

void Tile::setOwner(int Value)
{
	Owner = Value;	
}

//==================================

int Tile::getOwner()
{
	return Owner;
}

//==================================

void Tile::setArmyIndex(int Value)
{
	ArmyIndex = Value;
}

//==================================

int Tile::getArmyIndex()
{
	return ArmyIndex;
}

//==================================

void Tile::setPosition(sf::Vector2f NewPosition)
{
	Position = NewPosition;
}

//==================================

sf::Vector2f Tile::getPosition()
{
	return Position;
}