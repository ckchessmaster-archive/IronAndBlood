#include "Errors.h"

#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>

namespace Pixel
{
	//Prints out an error message and exits the game
	void fatalError(std::string errorString)
	{
		std::cout << errorString << std::endl;
		std::cout << "Enter any key to quit...";
		int tmp;
		std::cin >> tmp;
		SDL_Quit();
		exit(69);
	}//end fatalError
}