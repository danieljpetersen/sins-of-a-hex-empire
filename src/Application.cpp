#include "Application.h"
#include "Constants.h"
#include "Board.h"
#include <sstream>
#include <list>

#include <iostream>

namespace SFML_Wrapper
{
	CoreApplication::CoreApplication()
	{
	}
	
//==================================	
	
	void CoreApplication::initCoreFunctionality()
	{
		ProgramTitle = "Sins of a Hex Empire -- CLICK ON A STAR (CAPITAL) TO SELECT YOUR NATION -- SPACEBAR TO GENERATE NEW MAP";

		sf::ContextSettings ContextSetter(0,0,16);//16 anti alias

		this->setBackgroundColor(60, 90, 150);
		
		const int DEFAULT_SCREEN_WIDTH = 800;
		const int DEFAULT_SCREEN_HEIGHT = 500;
		MainWindow.create(sf::VideoMode({DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT}, 32), ProgramTitle, sf::Style::Close, ContextSetter);

		GameCamera.reset(sf::FloatRect({0.0f, 0.0f}, {(float)DEFAULT_SCREEN_WIDTH, (float)DEFAULT_SCREEN_HEIGHT}));
		
		this->updateScreenSizes();

		MainWindow.setView(GameCamera);

		GlobalAppFunctions.ProgramState = ChooseNation;

		Input.init(this);
		
		//Files.init();

		//don't remove next two lines, it's there to prevent the screen being white while loading
		this->clearScreen();
		this->refreshScreen();

		MainWindow.setFramerateLimit(30);	
	}
	
//==================================
		
	void CoreApplication::clearScreen()
	{
		MainWindow.clear(sf::Color(BackgroundColorRed, BackgroundColorGreen, BackgroundColorBlue));
	}
	
//==================================
 
	void CoreApplication::refreshScreen()
	{
		MainWindow.display();
	}
	
//==================================
 
	void CoreApplication::setBackgroundColor(int r, int g, int b)
	{
		BackgroundColorRed = r;
		BackgroundColorGreen = g;
		BackgroundColorBlue = b;
	}
	
//==================================

	void CoreApplication::setProgramTitle(std::string NewTitle)
	 {
		 ProgramTitle = NewTitle;
		 MainWindow.setTitle(ProgramTitle);
	 }

//==================================

	void CoreApplication::setCameraCenter(sf::Vector2f Position, Game_System *Game)
	{
		GameCamera.setCenter(Position);
		MainWindow.setView(GameCamera);
		GlobalAppFunctions.CameraCenter = Position;
	}

//==================================
 
	void CoreApplication::handleScreenResize()
	{
		GameCamera = sf::View(this->GameCamera.getCenter(), sf::Vector2f((float)this->MainWindow.getSize().x, (float)this->MainWindow.getSize().y));
		MainWindow.setView(this->GameCamera);
		
		sf::FloatRect NewWindowRect;
		NewWindowRect.left = 0;
		NewWindowRect.top = 0;
		NewWindowRect.height = (float)this->MainWindow.getSize().y;
		NewWindowRect.width = (float)this->MainWindow.getSize().x;
		
		this->updateScreenSizes();		
	}
	
//==================================
 
	void CoreApplication::updateScreenSizes()
	{		
		GlobalAppFunctions.WindowSize.x = (float)this->MainWindow.getSize().x;
		GlobalAppFunctions.WindowSize.y = (float)this->MainWindow.getSize().y;

		GlobalAppFunctions.HalfWindowSize.y = (float)this->MainWindow.getSize().x / 2;
		GlobalAppFunctions.HalfWindowSize.y = (float)this->MainWindow.getSize().y / 2;

		GlobalAppFunctions.CameraSize = this->GameCamera.getSize();
		GlobalAppFunctions.HalfCameraSize.x = this->GameCamera.getSize().x / 2;
		GlobalAppFunctions.HalfCameraSize.y = this->GameCamera.getSize().y / 2;

		GlobalAppFunctions.CameraCenter = this->GameCamera.getCenter();
	}

//==================================

	void CoreApplication::updateTitleWithFPS()
	{
		//Adds FPSCount to Titlebar
		std::stringstream NewTitle;
		NewTitle << ProgramTitle;
		NewTitle << " -- FPS:  ";
		NewTitle << FrameRate.getFPS();

		MainWindow.setTitle(NewTitle.str());
	}
	
//==================================
//==================================
//================================== GlobalApplicationFunctions
//==================================
//==================================
	
	GlobalApplicationFunctions::GlobalApplicationFunctions()
	{
	}
	
//==================================

	GlobalApplicationFunctions::~GlobalApplicationFunctions()
	{
	}
	
//==================================
	
	void GlobalApplicationFunctions::setCameraCenter(sf::Vector2f NewCenter)
	{
		CameraCenter = NewCenter;
	}
	
//==================================
	
	sf::Vector2f GlobalApplicationFunctions::getCameraCenter()
	{
		return CameraCenter;
	}
	
//==================================
 
	sf::Vector2f GlobalApplicationFunctions::getCameraSize()
	{
		return CameraSize;
	} 
			
//==================================
 
	sf::Vector2f GlobalApplicationFunctions::getHalfCameraSize()
	{
		return HalfCameraSize;
	} 
			
//==================================
 
	bool GlobalApplicationFunctions::isPointInsideCamera(sf::Vector2f Point)
	{
		float xMin = CameraCenter.x - HalfCameraSize.x;
		float xMax = CameraCenter.x + HalfCameraSize.x;
		float yMin = CameraCenter.y - HalfCameraSize.y;
		float yMax = CameraCenter.y + HalfCameraSize.y;

		if (Point.x >= xMin) 
			if (Point.y >= yMin) 
				if (Point.x <= xMax) 
					if (Point.y <= yMax)
					{
						return true;
					}

		return false;
	}

//==================================
	
	sf::Vector2f GlobalApplicationFunctions::getWindowSize()
	{
		return WindowSize;
	}
	
//==================================
	
	sf::Vector2f GlobalApplicationFunctions::getHalfWindowSize()
	{
		return HalfWindowSize;
	}
	
//==================================

	int GlobalApplicationFunctions::getPercent(float Percent, float Number) 
	{
		float Answer = (Percent / 100) * Number;
		return (int)Answer;
	}
		
//==================================

	bool GlobalApplicationFunctions::DoLineSegmentsIntersect(sf::Vector2f Point1, sf::Vector2f Point2,
								 sf::Vector2f Point3, sf::Vector2f Point4) 
	{ 
	  float bx = Point2.x - Point1.x; 
	  float by = Point2.y - Point1.y; 
	  float dx = Point4.x - Point3.x; 
	  float dy = Point4.y - Point3.y;
	  float b_dot_d_perp = bx * dy - by * dx;
	  if(b_dot_d_perp == 0) {
		return false;
	  }
	  float cx = Point3.x - Point1.x;
	  float cy = Point3.y - Point1.y;
	  float t = (cx * dy - cy * dx) / b_dot_d_perp;
	  if(t < 0 || t > 1) {
		return false;
	  }
	  float u = (cx * by - cy * bx) / b_dot_d_perp;
	  if(u < 0 || u > 1) { 
		return false;
	  }

	  return true;
	}

//==================================

	bool GlobalApplicationFunctions::isPointInsideRect(sf::Vector2f xy, sf::IntRect RectToTest)
	{
		int xMax = RectToTest.left + RectToTest.width;
		int yMax = RectToTest.top + RectToTest.height;

		if ((xy.x				> RectToTest.left) &&
			(xy.y				> RectToTest.top) &&
			(xy.x				< xMax) &&
			(xy.y				< yMax)) 
		{
			return true;
		}

		return false;
	}

//==================================

	void GlobalApplicationFunctions::centerPosition(int Start, int AreaSize, int ElementSize, int &ValueToReturn)
	{
		ValueToReturn = int(Start + (AreaSize / 2.0f) - (ElementSize / 2.0f));
	}

//==================================

	void GlobalApplicationFunctions::centerPosition(float Start, float AreaSize, float ElementSize, float &ValueToReturn)
	{
		ValueToReturn = float(Start + (AreaSize / 2.0f) - (ElementSize / 2.0f));
	}

//==================================

	bool GlobalApplicationFunctions::isPointWithinArea(int xPoint, int yPoint, int AreaLeft, int AreaTop, int Height, int Width)
	{
		if ((xPoint > AreaLeft) &&
			(yPoint > AreaTop) &&
			(yPoint < AreaTop + Height) &&
			(xPoint < AreaLeft + Width)) 
		{
			return true;
		}
	
		return false;
	}
 }