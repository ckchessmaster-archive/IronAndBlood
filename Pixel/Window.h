#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <string>

namespace Pixel
{
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window
	{
	private:
		SDL_Window* _sdlWindow;
		int _screenWidth;
		int _screenHeight;

	public:
		Window();
		~Window();

		int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);

		void swapBuffer();

		//Getters
		int getScreenWidth() { _screenWidth; }
		int getScreenHeight() { _screenHeight; }
	};
}
