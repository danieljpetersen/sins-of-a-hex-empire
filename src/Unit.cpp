#include "Unit.h"
#include "Constants.h"

Unit::Unit()
{
	this->resetUnit();
}

void Unit::resetUnit()
{
	Position.x = -1000;
	Position.y = -1000;
	ArmySize = 0;
	Experience = 0;
	DidUnitMove = false;
	IsUnitActive = true;
	TileIndex = -1;
}

//==================================

void Unit::addArmySize(int Value)
{
	this->setArmySize(Value + ArmySize);
}

//==================================

void Unit::setArmySize(int Value)
{
	if (Value <= MAX_ARMY_SIZE)
		ArmySize = Value;
	else
	{
		ArmySize = MAX_ARMY_SIZE;
	}

	if (ArmySize == 0)
	{
		this->resetUnit();
		this->setIsUnitActive(false);
	}
}

//==================================

int Unit::getArmySize()
{
	return ArmySize;
}

//==================================

void Unit::addExperience(int Value)
{
	this->setExperience(Value + Experience);
}

//==================================
void Unit::setExperience(int Value)
{
	if (Value < 0)
		Value = 0;

	if (Value <= MAX_EXPERIENCE)
		Experience = Value;
	else
	{
		Experience = MAX_EXPERIENCE;
	}
}

//==================================

int Unit::getExperience()
{
	return Experience;
}

//==================================

void Unit::setDidUnitMove(bool Value)
{
	DidUnitMove = Value;
}

//==================================

bool Unit::getDidUnitMove()
{
	return DidUnitMove;
}

//==================================

void Unit::setIsUnitActive(bool Value)
{
	if (Value == false)
		this->resetUnit();
	IsUnitActive = Value;
}

//==================================

bool Unit::getIsUnitActive()
{
	return IsUnitActive;
}

//==================================

void Unit::setPosition(sf::Vector2f Value)
{
	Position = Value;
}

//==================================

sf::Vector2f Unit::getPosition()
{
	return Position;
}

//==================================

void Unit::setTile(int Value)
{
	TileIndex = Value;
}

//==================================

int Unit::getTile()
{
	return TileIndex;
}
