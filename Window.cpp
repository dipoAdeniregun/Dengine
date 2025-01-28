#include "Window.h"
#include "Errors.h"

namespace Dengine {
	int Window::createWindow(std::string _windowName, int _screenWidth, int _screenHeight, unsigned int _currentFlags) {
		screenWidth = _screenWidth;
		screenHeight = _screenHeight;

		//set SDL flags
		Uint32 flags = SDL_WINDOW_OPENGL;

		if (_currentFlags & INVISIBLE) { flags |= SDL_WINDOW_HIDDEN; }
		if (_currentFlags & FULLSCREEN) { flags |= SDL_WINDOW_FULLSCREEN_DESKTOP; }
		if (_currentFlags & RESIZABLE) { flags |= SDL_WINDOW_RESIZABLE; }
		if (_currentFlags & BORDERLESS) { flags |= SDL_WINDOW_BORDERLESS; }

		sdlWindow = SDL_CreateWindow(_windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, flags);

		//confirm window is opened
		if (sdlWindow == nullptr) {
			fatalError("SDL window could not be created!");
		}

		SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindow);

		//confirm context is created
		if (glContext == nullptr) {
			fatalError("Context couldn't be created!");
		}

		GLenum error = glewInit();

		//confirm proper initialization
		if (error != GLEW_OK) {
			fatalError("Could not initialize GLEW!");
		}

		//Set the background color to blue
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		//turn on vsync
		//SDL_GL_SetSwapInterval(0);

		//enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swapBuffer() {
		SDL_GL_SwapWindow(sdlWindow);	//swap buffer(window) being drawn to (check setAttribute in init)
	}

}
