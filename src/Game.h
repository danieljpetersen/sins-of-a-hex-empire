#ifndef MY_GAME_H
#define MY_GAME_H

#include "Board.h"
#include "Nation.h"
#include "Pathfinding.h"

namespace SFML_Wrapper
{
	class CoreApplication;
}
class Game_System
{
private:
	int HumanNationIndex, TurnNumber;
	int ActivePlayer, SelectedTile;

	int TilesInRange[18];//for movement

public:
	Game_System(SFML_Wrapper::CoreApplication *TheApp);
	
	Pathfinding Pathfinder;
	
	Board_Component Board;

	SFML_Wrapper::CoreApplication *App;

	std::vector<Nation> Players;
	
	void spawnArmies(int PlayerIndex);
	bool moveArmy(int PlayerIndex, int UnitIndex, int NewTile);
	bool initiateAttack(int AttackingPlayer, int AttackingArmy, int DefendingPlayer, int DefendingArmy);
	void removeArmyFromTile(int TileIndex);
	bool addArmyToTile(int Tile, int PlayerIndex, int ArmyIndex, bool &DidEntireArmyTransfer);
	int findFreeArmyToAdd(int Player);

	void endTurn();
	int getTurn();

	int getHumanNationIndex();
	void setHumanNation(int Value);

	void addInitialArmies();

	void setActivePlayer(int Value);
	int getActivePlayer();
	
	void setSelectedTile(int Value);
	int getSelectedTile();

	bool isHexInRange(int TileClicked);
	void determineTilesInRange();
	int getTilesInMovementRange(int CenterTile);
	void resetTilesInMovementRange();
	bool isTileInTilesInRange(int TileIndex);

	void expandBorders(int HexMovedTo, int PlayerIndex);

	void killPlayerOff(int PlayerToKill, int PlayerGainingTerritories);

	void generateMap();
};
 
#endif 