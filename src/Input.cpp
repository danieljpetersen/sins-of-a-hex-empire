#include "Input.h"
#include "Game.h"
#include "Board.h"
#include "Application.h"
#include "Constants.h"

Input_System::Input_System()
{
	App = nullptr;
	this->resetVariables();
}

//==================================

void Input_System::init(SFML_Wrapper::CoreApplication *TheApp)
{
	App = TheApp;
}

//==================================

void Input_System::handleBasicInput(bool &quit)
{
	resetVariables();

	while (App->MainWindow.pollEvent(event))
    {
		if (event.type == sf::Event::KeyPressed)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				SpacePressed = true;			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				EnterPressed = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket))
				LBracketPressed = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket))
				RBracketPressed = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
				BackspacePressed = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
				BPressed = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
				IPressed = true;
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad9))
				NumPad9Pressed = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8))
				NumPad8Pressed = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7))
				NumPad7Pressed = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))
				NumPad6Pressed = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))
				NumPad5Pressed = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
				NumPad4Pressed = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
				NumPad3Pressed = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
				NumPad2Pressed = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
				NumPad1Pressed = true;
		}

		
		if (event.type == sf::Event::MouseWheelScrolled)
		{
			if (event.mouseWheelScroll.delta > 0)
				ScrolledUp = true;
			else if (event.mouseWheelScroll.delta < 0)
				ScrolledDown = true;
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Button::Right)
				RightMouseClicked = true;	
			else if (event.mouseButton.button == sf::Mouse::Button::Left)
				LeftMouseClicked = true;				
		}
		if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Button::Right)
				RightMouseReleased = true;
		}

		WindowMousePosition = sf::Mouse::getPosition(App->MainWindow); 
		sf::Vector2f MousePos = App->MainWindow.mapPixelToCoords(WindowMousePosition);
		MousePosition.x = (int)MousePos.x;
		MousePosition.y = (int)MousePos.y;
				
		if (event.type == sf::Event::Resized)	
		{
			WasScreenResized = true;
		}
		
        if (event.type == sf::Event::Closed)
        {
			quit = true;
			return;
		}		
    }
	
	//===

	if (WasScreenResized == true)
		App->handleScreenResize();
	
	/*if (ScrolledUp == true)
	{
		App->GameCamera.zoom(.9f);
		App->MainWindow.setView(App->GameCamera);
		App->updateScreenSizes();
	}

	if (ScrolledDown == true)
	{
		App->GameCamera.zoom(1.1f);
		App->MainWindow.setView(App->GameCamera);
		App->updateScreenSizes();
	}*/
}

//==================================

void Input_System::resetVariables()
{
	WasScreenResized = false;

	RightMouseReleased = false;

	ScrolledUp = false;
	ScrolledDown = false;

	DoubleClicked = false;
	LeftMouseClicked = false;
	RightMouseClicked = false;

	SpacePressed = false;
	BackspacePressed = false;
	EnterPressed = false;
	BPressed = false;
	IPressed = false;
	
	LBracketPressed = false;
	RBracketPressed = false;

	PlusPressed = false;
	MinusPressed = false;
	
	NumPad9Pressed = false;
	NumPad8Pressed = false;
	NumPad7Pressed = false;
	NumPad6Pressed = false;
	NumPad5Pressed = false;
	NumPad4Pressed = false;
	NumPad3Pressed = false;
	NumPad2Pressed = false;
	NumPad1Pressed = false;	
}

//==================================

sf::Vector2i Input_System::getWindowMousePosition()
{
	return WindowMousePosition;
}

//==================================

sf::Vector2i Input_System::getMousePosition()
{
	return MousePosition;
}

//==================================

void Input_System::handleMouseInput(Game_System &Game)
{		
	bool AreWeOnGUI = false;
	//AreWeOnGUI = TheGUI.handleNormalScreenGUI(WindowMousePosition.x, WindowMousePosition.y, LeftMouseClicked);

	if (AreWeOnGUI != true)
	{
		if (LeftMouseClicked)
		{
			if (GlobalAppFunctions.ProgramState == Normal)
				this->handleLeftMouseClickNormalState(Game, findTileMouseIsOver(Game.Board));
			if (GlobalAppFunctions.ProgramState == ChooseNation)
				handleLeftMouseClickChooseNationState(Game, findTileMouseIsOver(Game.Board));
		}

		if (RightMouseClicked)
			if (GlobalAppFunctions.ProgramState == ChooseNation)
			{
				Game.addInitialArmies();
				App->setProgramTitle("Sins of a Hex Empire -- SPACEBAR TO GENERATE NEW MAP -- ENTER TO END TURN");
			}

		if (SpacePressed)
			Game.generateMap();//handleRightMouseClickNormalState(Game, findTileMouseIsOver(Game.Board));
	}
}

//==================================

void Input_System::handleLeftMouseClickNormalState(Game_System &Game, int TileClicked)
{
	if (Game.Board.isValidTile(TileClicked))
	{		
		//deselect the current tile, but save the it as well as any tiles currently in range
		int SelectedTileAtStart = Game.getSelectedTile();
		int OriginalTilesInRange[18];
		for (int i = 0; i < 18; i++)
			OriginalTilesInRange[i] = Game.getTilesInMovementRange(i);

		Game.resetTilesInMovementRange();
		Game.setSelectedTile(-1);
			
		//if we clicked on a tile containing one of our units, make it our new tile (provided it wasn't the tile originally selected -- in that case, we leave nothing selected)
		bool NewUnitSelected = false;
		if (Game.Board.Tiles[TileClicked].getOwner() == Game.getHumanNationIndex())
			if (Game.Board.Tiles[TileClicked].getArmyIndex() != -1)
				if (SelectedTileAtStart != TileClicked)
					if (!Game.Players[Game.Board.Tiles[TileClicked].getOwner()].Units[Game.Board.Tiles[TileClicked].getArmyIndex()].getDidUnitMove())
					{
						//if we have a unit selected, don't select a new unit if the unit clicked is within range
						if (SelectedTileAtStart != -1)
						{
							bool InRange = false;
							for (int i = 0; i < 18; i++)
								if (OriginalTilesInRange[i] == TileClicked)
								{
									InRange = true;
								}

							if (!InRange)
							{
								Game.setSelectedTile(TileClicked);
								Game.determineTilesInRange();					
								NewUnitSelected = true;
							}
						}
						else
						{
							Game.setSelectedTile(TileClicked);
							Game.determineTilesInRange();					
							NewUnitSelected = true;
						}
					}				
		
		//if no unit was selected during this function, let's check to see if we should move the currently selected unit
		if (!NewUnitSelected)
		{
			if (SelectedTileAtStart != -1)
			{
				for (int i = 0; i < 18; i++)
				{
					if (OriginalTilesInRange[i] != -1)
					{
						if (OriginalTilesInRange[i] == TileClicked)
						{
							int PlayerIndex = Game.Board.Tiles[SelectedTileAtStart].getOwner();
							int ArmyIndex = Game.Board.Tiles[SelectedTileAtStart].getArmyIndex();
							Game.moveArmy(PlayerIndex, ArmyIndex, TileClicked);
							break;
						}
					}
				}
			}
		}
	}
	

	GlobalAppFunctions.UpdateTiles = true;
}

//==================================

void Input_System::handleRightMouseClickNormalState(Game_System &Game, int TileClicked)
{
}

//==================================

void Input_System::handleLeftMouseClickChooseNationState(Game_System &Game, int TileClicked)
{
	if (Game.Board.isValidTile(TileClicked))
	{
		if (Game.Board.Tiles[TileClicked].getOwner() != -1)
		{
			Game.setHumanNation(Game.Board.Tiles[TileClicked].getOwner());
			Game.addInitialArmies();
			App->setProgramTitle("Sins of a Hex Empire -- SPACEBAR TO GENERATE NEW MAP -- ENTER TO END TURN");
		}
	}
}
//==================================

int Input_System::findTileMouseIsOver(Board_Component &Board)
{
	int xClick = MousePosition.x;
	int yClick = MousePosition.y;

	sf::Vector2f Point1;
	sf::Vector2f Point2;
	sf::Vector2f Point3;
	sf::Vector2f Point4;
	
	for (unsigned int i = 0; i < Board.NUMBER_OF_TILES; i++)
	{	
		if (Board.isPointInsideTile(xClick, yClick, i) == true)
			return i;
	}
	return -1;
}

//==================================

void Input_System::handleKeyboardInput(Game_System &Game)
{	
	if (GlobalAppFunctions.ProgramState == Normal)
		if (EnterPressed == true)
		{
			Game.endTurn();
		}
}