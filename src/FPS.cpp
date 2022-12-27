#include "FPS.h"

FPS::FPS()
{
	FramesDrawn = 0;
	FramesPerSecond = 0;
	FPSTimer.restart();

	TimeElapsed.restart();
	NewTime = 0.0f;
	DeltaTime = 0.0f;
	CurrentTime = 0.0f;
	Accumulator = 0.0f;

	dt = 0.1f;
	t = 0.0f;
}

//==================================
 
void FPS::update()
{
	//update fps timer
	if (FPSTimer.getElapsedTime().asSeconds() >= 1)
	{
		FramesPerSecond = (int)(FramesDrawn / FPSTimer.getElapsedTime().asSeconds());
		FramesDrawn = 0;
		FPSTimer.restart();
	}
	FramesDrawn++;

	//update frame independent flow variables
	NewTime = TimeElapsed.getElapsedTime().asSeconds();
	DeltaTime = NewTime - CurrentTime;
	CurrentTime = NewTime;
 
	if (DeltaTime > 0.25f)
		DeltaTime = 0.25f;

	Accumulator += DeltaTime;
}

//==================================
 
const unsigned int FPS::getFPS()
{
	return FramesPerSecond;
}

//==================================
 
const bool FPS::timeForNextGameTic()
{
	if (Accumulator >= dt)
	{
		Accumulator -= dt;

		return true;
	}

	return false;
}

//==================================


void FPS::timeStepEnd()
{
	t += dt;
}