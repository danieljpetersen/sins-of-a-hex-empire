#ifndef INPUT_H
#define INPUT_H

#include <SFML/Graphics.hpp>

class Game_System;
class Board_Component;
namespace SFML_Wrapper
{
	class CoreApplication;
};

class Input_System
{
private:
	SFML_Wrapper::CoreApplication *App;

	sf::Event event;	

	bool WasScreenResized, ScrolledUp, ScrolledDown, DoubleClicked, LeftMouseClicked, RightMouseClicked;
	bool SpacePressed, BackspacePressed, EnterPressed, BPressed, IPressed, LBracketPressed, RBracketPressed, MinusPressed, PlusPressed;
	bool NumPad9Pressed, NumPad8Pressed, NumPad7Pressed, NumPad6Pressed, NumPad5Pressed, NumPad4Pressed, NumPad3Pressed, NumPad2Pressed, NumPad1Pressed;
	bool RightMouseReleased;

	sf::Vector2i MousePosition, WindowMousePosition;

	void resetVariables();

	void handleLeftMouseClick();

	void handleLeftMouseHeldDown();	

	void handleLeftMouseClick(Game_System &Game, int TileClicked);
	
	void handleRightMouseClick(Game_System &Game, int TileClicked);

	void handleLeftMouseClickNormalState(Game_System &Game, int TileClicked);
	void handleRightMouseClickNormalState(Game_System &Game, int TileClicked);
	
	void handleLeftMouseClickChooseNationState(Game_System &Game, int TileClicked);

	void handleMouseInputCityScreen(Game_System &Game);
	
public:
	Input_System();

	void init(SFML_Wrapper::CoreApplication *TheApp);
	
	void handleBasicInput(bool &quit);
	
	int findTileMouseIsOver(Board_Component &Board);
	
	void handleMouseInput(Game_System &Game);

	void handleKeyboardInput(Game_System &Game);

	void handleTimeSensitiveKeyboardInput(Game_System &Game, float Interpolation);
	
	bool getScrolledUp();

	bool getScrolledDown();	

	sf::Vector2i getWindowMousePosition();
	
	sf::Vector2i getMousePosition();

	int DEBUG_TILE;
};

#endif