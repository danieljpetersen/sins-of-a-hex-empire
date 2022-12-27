#include <SFML/Graphics.hpp>
#include "Application.h"
#include "Game.h"

SFML_Wrapper::GlobalApplicationFunctions GlobalAppFunctions;

int main()
{	
	srand((long)time(0));

	SFML_Wrapper::CoreApplication App;

	App.initCoreFunctionality();
	
	Game_System Game(&App);	
	App.Draw.init(&App, Game);

	bool quit = false;
	while (quit == false)
	{
		App.FrameRate.update();   
		App.updateTitleWithFPS(); 

		App.Input.handleBasicInput(quit);				
		
		App.Input.handleMouseInput(Game);		
		
		App.Input.handleKeyboardInput(Game);	
		
		App.clearScreen();
		
		App.Draw.drawScene(Game);

		App.refreshScreen();
	}

	return 0;
}		
