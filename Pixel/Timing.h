#pragma once

namespace Pixel {

	class FpsLimiter {
	private:
		float _fps;
		float _maxFPS;
		float _frameTime;
		unsigned int _startTicks;

		void calculateFPS();

	public:
		FpsLimiter();
		void init(float maxFPS);

		void setMaxFPS(float maxFPS);

		void begin();
		
		// end will return the current FPS
		float end();
	};//end FpsLimiter
}//end namespace