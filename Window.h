#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

namespace Dengine {
	enum WindowFlags {
		INVISIBLE = 0x1,
		FULLSCREEN = 0x2,
		RESIZABLE = 0x8,
		BORDERLESS = 0x4
	};


	class Window
	{
	private:
		SDL_Window* sdlWindow;
		int screenWidth;
		int screenHeight;
	public:
		//Window();
		int createWindow(std::string _windowName, int _screenWidth, int _screenHeight, unsigned int _currentFlags);

		int getScreenWidth() { return screenWidth; }
		int getScreenHeight() { return screenHeight; }

		void setScreenSize(int w, int h) { screenWidth = w; screenHeight = h; }
		void swapBuffer();
	};

}