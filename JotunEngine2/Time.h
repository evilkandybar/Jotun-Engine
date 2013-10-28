#pragma once
#include "stdafx.h"
/*!\brief Manages the time for the engine*/
class Time {
public:
	/*!\brief Synchronizes the time with GLFW's time coutner*/
	static void init();
	/*!\brief Updates the time and the timestep, using GLFW to gague the amount of time which has 
	passed*/
	static void update();
	/*!\brief Returns the current time
	
	\return The current time*/
	static float getTime();
	/*!\brief Returns the time it took to complete the last frame
	
	\return The amount of time ti took to complete the last frame*/
	static float getDeltaTime();
private:
	static float lastTime, curTime, deltaTime;
};