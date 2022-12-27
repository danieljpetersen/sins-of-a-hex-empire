#ifndef FPS_HPP__INCLUDED
#define FPS_HPP__INCLUDED
 
#include <SFML/System.hpp>

class FPS
{
private:
	unsigned int FramesDrawn;	
	unsigned int FramesPerSecond;	
	sf::Clock FPSTimer;
	
	//remaining variables are for frame independent proram flow	
	sf::Clock TimeElapsed;
	float NewTime;
	float DeltaTime;
	float CurrentTime;
	float Accumulator;

	float dt;
	float t;

public:
	FPS();

 	void update();
	
	const unsigned int getFPS();
	
	const bool timeForNextGameTic();
	void timeStepEnd();
	
};
 
#endif 