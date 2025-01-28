#include "Errors.h"
#include <iostream>
#include <sdl/SDL.h>
#include <cstdlib>

namespace Dengine {
	//print error message and exit on fatal error
	void fatalError(const std::string& errorMessage) {
		std::cout << errorMessage << std::endl;
		std::cout << "Enter any key to quit: ";
		int temp;
		std::cin >> temp;
		SDL_Quit();
		exit(69);
	}
}