#ifndef SFML_PROGRAM_H
#define SFML_PROGRAM_H

#include "Drawing.h"
#include "Input.h"
#include "FPS.h"
#include <string>
#include <chrono>
#include <random>

enum States { Normal, ChooseNation };

class RandomNumberGenerator {
 public:
  RandomNumberGenerator() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator_.seed(seed);
  }

  int getRandomInt(int low, int high) {
    std::uniform_int_distribution<int> distribution(low, high);
    return distribution(generator_);
  }

 private:
  std::mt19937 generator_;
};

namespace SFML_Wrapper
{
	
	class GlobalApplicationFunctions 
	{
	private:
		sf::Vector2f WindowSize;
		sf::Vector2f HalfWindowSize;

		sf::Vector2f CameraCenter;
		sf::Vector2f CameraSize;
		sf::Vector2f HalfCameraSize;

		void setCameraCenter(sf::Vector2f NewCenter);

	public:
		GlobalApplicationFunctions();
		~GlobalApplicationFunctions();

		RandomNumberGenerator Rand;

		int getPercent(float Percent, float Number);
		
		bool isPointWithinArea(int xPoint, int yPoint, int AreaLeft, int AreaTop, int Height, int Width);

		void centerPosition(float Start, float AreaSize, float ElementSize, float &ValueToReturn);

		void centerPosition(int Start, int AreaSize, int ElementSize, int &ValueToReturn);

		bool isPointInsideRect(sf::Vector2f xy, sf::IntRect RectToTest);

		bool DoLineSegmentsIntersect(sf::Vector2f Point1, sf::Vector2f Point2, sf::Vector2f Point3, sf::Vector2f Point4);
		
		//==================================		

		sf::Vector2f getCameraCenter();
		
		sf::Vector2f getCameraSize();

		sf::Vector2f getHalfCameraSize();

		bool isPointInsideCamera(sf::Vector2f Point);

		sf::Vector2f getWindowSize();

		sf::Vector2f getHalfWindowSize();

		States ProgramState;
		
		//==================================

		bool UpdateTiles;

		friend class CoreApplication;
	};

	class CoreApplication
	{
	private:
		std::string ProgramTitle;
		
		int BackgroundColorRed, BackgroundColorGreen, BackgroundColorBlue, ScreenWidth, ScreenHeight, HalfScreenWidth, HalfScreenHeight, HalfCameraWidth, HalfCameraHeight;

	public:
		CoreApplication();		

		void initCoreFunctionality();
		
		sf::RenderWindow MainWindow;
	
		Drawing Draw;

		sf::View GameCamera;

		Input_System Input;		
		
		FPS FrameRate;
		
		//==================================
						
		void clearScreen();		

		void refreshScreen();
		
		void setBackgroundColor(int r, int g, int b);

		void setProgramTitle(std::string NewTitle);

		void setCameraCenter(sf::Vector2f Position, Game_System *Game);
	
		void switchToWindowCamera();

		void switchToGameCamera();

		void handleScreenResize();

		void updateScreenSizes();
		
		void updateTitleWithFPS();		
	};
}

extern SFML_Wrapper::GlobalApplicationFunctions GlobalAppFunctions;

#endif