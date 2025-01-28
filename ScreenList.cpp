#include "ScreenList.h"

#include "IGameScreen.h"
#include "IMainGame.h"

namespace Dengine {
	Dengine::ScreenList::ScreenList(IMainGame* game):
		m_game(game)
	{
	}

	ScreenList::~ScreenList()
	{
		destroy();
	}

	IGameScreen* Dengine::ScreenList::moveNext()
	{
		IGameScreen* currentScreen = getCurrentScreen();

		if (currentScreen->getNextScreenIdx() != SCREEN_IDX_NO_SCREEN) {
			m_currentScreenIdx = currentScreen->getNextScreenIdx();
		}
		return m_screens[m_currentScreenIdx];
	}

	IGameScreen* Dengine::ScreenList::movePrevious()
	{
		IGameScreen* currentScreen = getCurrentScreen();
		if (currentScreen->getPreviousScreenIdx() != SCREEN_IDX_NO_SCREEN) {
			m_currentScreenIdx = currentScreen->getPreviousScreenIdx();
		}
		return m_screens[m_currentScreenIdx];
	}

	void Dengine::ScreenList::setScreen(int nextScreen)
	{
		m_currentScreenIdx = nextScreen;
	}

	void Dengine::ScreenList::addScreen(IGameScreen* newScreen)
	{
		newScreen->m_screenIdx = m_screens.size();
		m_screens.push_back(newScreen);
		newScreen->build();
		newScreen->setParentGame(m_game);
	}

	IGameScreen* ScreenList::getCurrentScreen()
	{
		if (m_currentScreenIdx == -1)
		{
			return nullptr;
		}
		return m_screens[m_currentScreenIdx];
	}

	void Dengine::ScreenList::destroy()
	{
		for (size_t i = 0; i < m_screens.size(); i++) {
			m_screens[i]->destroy();
		}
		m_screens.resize(0);
		m_currentScreenIdx = SCREEN_IDX_NO_SCREEN;
	}
}