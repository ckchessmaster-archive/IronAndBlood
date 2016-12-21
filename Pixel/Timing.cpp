#include "Timing.h"

#include <SDL/SDL.h>

namespace Pixel {
	FpsLimiter::FpsLimiter() {

	}//end Constructor

	void FpsLimiter::init(float maxFPS) {
		setMaxFPS(maxFPS);
	}//end init

	void FpsLimiter::setMaxFPS(float maxFPS) {
		_maxFPS = maxFPS;
	}//end setTargetFPS

	void FpsLimiter::begin() {
		_startTicks = SDL_GetTicks();
	}//end begin

	float FpsLimiter::end() {
		calculateFPS();
		
		float frameTicks = SDL_GetTicks() - _startTicks;
		//limit the fps to the max fps
		if (1000.0f / _maxFPS > frameTicks) {
			SDL_Delay(1000.0f / _maxFPS - frameTicks);
		}//end if

		return _fps;
	}//end end

	void FpsLimiter::calculateFPS() {
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int currentFrame = 0;

		static float prevTicks = SDL_GetTicks();

		float currentTicks;
		currentTicks = SDL_GetTicks();

		_frameTime = currentTicks - prevTicks;
		frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

		prevTicks = currentTicks;

		int count;

		currentFrame++;
		if (currentFrame < NUM_SAMPLES)
			count = currentFrame;
		else
			count = NUM_SAMPLES;
		//end if/else

		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++)
			frameTimeAverage += frameTimes[i];
		//end for

		frameTimeAverage /= count;

		//make sure we don't divide by 0
		if (frameTimeAverage > 0)
			_fps = 1000.0f / frameTimeAverage;
		else
			_fps = 60.0f;
		//end if/else
	}//end calculateFPS
}//end namespace