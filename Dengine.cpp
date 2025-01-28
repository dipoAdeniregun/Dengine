#include "Dengine.h"
#include <sdl/SDL.h>
#include <GL/glew.h>
#include "Errors.h"

namespace Dengine {
	int init() {
		//drawing to a window involves clearing then redrawing every frame
		//this creates two buffers for the window so when one buffer(window) is being drawn to,
		//the other is being cleared and it prevents flickeing and drawing is smoother
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		//initialize SDL
		int initError = SDL_Init(SDL_INIT_EVERYTHING);


		/*
		* do error checking here!!!
		*/
		if (initError) {
			fatalError("SDL Init Failed");
		}

		return 0;
	}
}