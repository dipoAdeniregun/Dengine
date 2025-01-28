#include "IMainGame.h"
#include "Timing.h"
#include "ScreenList.h"
#include "IGameScreen.h"

#include <iostream>

namespace Dengine {
	IMainGame::IMainGame()
	{
		m_screenList = std::make_unique<ScreenList>(this);
	}

	IMainGame::~IMainGame()
	{
		//Empty
	}

	void IMainGame::run()
	{
		if (!init()) return;

		FPSLimiter limiter;
		limiter.setMaxFPS(60.0f);

		m_isRunning = true;
		while (m_isRunning) {
			limiter.beginFrame();

			inputManager.update();
			update();

			if (m_isRunning) {
				draw();
				m_fps = limiter.endFrame();
				m_window.swapBuffer();
			}
			
		}
	}

	void IMainGame::exitGame()
	{
		m_currentScreen->onExit();
		if (m_screenList) {
			m_screenList->destroy();
			m_screenList.reset();
			std::cout << "IMAINGAME EXIT FINE\n";
		}
		m_isRunning = false;
	}

	bool IMainGame::init()
	{
		Dengine::init();
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		
		if (!initSystems()) return false;

		onInit();
		addScreens();

		m_currentScreen = m_screenList->getCurrentScreen();
		m_currentScreen->onEntry();
		m_currentScreen->setRunning();

		return true;
	}

	bool IMainGame::initSystems()
	{
		//std::cout << "INIT SYSTEMS\n";
		int flags =  RESIZABLE;
		int createWindowerror = m_window.createWindow("Default", 720, 540, flags);
		return true;
	}
	
	void IMainGame::update()
	{
		if (m_currentScreen) {
			switch (m_currentScreen->getState()) {
			
			case ScreenState::RUNNING:
				m_currentScreen->update();
				break;

			case ScreenState::CHANGE_NEXT:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->moveNext();
				if (m_currentScreen)
				{
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();

				}
				break;

			case ScreenState::CHANGE_PREV:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->movePrevious();
				if (m_currentScreen)
				{
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();

				}
				break;

			case ScreenState::EXIT_APP:
				exitGame();
				break;

			default:
				break;
			}
		}
		else {
			exitGame();
		}
	}
	
	void IMainGame::draw()
	{
		glViewport(0, 0, m_window.getScreenWidth(), m_window.getScreenHeight());
		if (m_currentScreen && m_currentScreen->getState() == ScreenState::RUNNING) {
			m_currentScreen->draw();
		}
	}
	
	void IMainGame::onSDLEvent(SDL_Event& evnt)
	{
		
		switch (evnt.type) {
			case SDL_QUIT:
				exitGame();
				break;
			case SDL_MOUSEMOTION:
				inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
				break;
			case SDL_KEYDOWN:
				//std::cout << char(evnt.key.keysym.sym) <<" Key Pressed\n";
				inputManager.pressKey(evnt.key.keysym.sym);
				break;
			case SDL_KEYUP:
				inputManager.releaseKey(evnt.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				inputManager.pressKey(evnt.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				inputManager.releaseKey(evnt.button.button);
				break;
			case SDL_WINDOWEVENT:
				if (evnt.window.event == SDL_WINDOWEVENT_RESIZED) {
					//std::cout << "Detected Window Resizing\n";
					m_window.setScreenSize(evnt.window.data1, evnt.window.data2);
					resizeCamera = true;
				}
				break;
			case SDL_MOUSEWHEEL:
				//std::cout << "Mouse wheel scrolled by " << evnt.wheel.y << "\n";
				inputManager.setMouseScrolled(evnt.wheel.x, evnt.wheel.y);
		}
		
	}
}
