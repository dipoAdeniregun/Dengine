#pragma once
#include <vector>


namespace Dengine {
	class IGameScreen;
	class IMainGame;

	class ScreenList
	{
	public:
		ScreenList(IMainGame* game);
		~ScreenList();
		
		IGameScreen* moveNext();
		IGameScreen* movePrevious();

		void setScreen(int nextScreen);
		void addScreen(IGameScreen* newScreen);

		IGameScreen* getCurrentScreen();

		void destroy();

	protected:
		IMainGame* m_game = nullptr;
		std::vector<IGameScreen*> m_screens;
		int m_currentScreenIdx = -1;

	};

}