#pragma once

#define SCREEN_IDX_NO_SCREEN -1

namespace Dengine {
	class IMainGame;

	enum class ScreenState {
		NONE,
		RUNNING,
		EXIT_APP,
		CHANGE_NEXT,
		CHANGE_PREV
	};

	class IGameScreen
	{
		friend class ScreenList;
	public:
		IGameScreen(){/*Empty*/ }
		virtual ~IGameScreen(){/*Empty*/ }

		// called when a screen is created or destroyed
		virtual void build() = 0;		
		virtual void destroy() = 0;

		//called when you enter or exit a screen
		virtual void onEntry() = 0;
		virtual void onExit() = 0;

		//called in main game loop;
		virtual void update() = 0;
		virtual void draw() = 0;

		//returns our current, next and previous screen indexes 
		int getCurrentScreenIdx() const { return m_screenIdx; }
		virtual int getNextScreenIdx() = 0;
		virtual int getPreviousScreenIdx() = 0;

		void setParentGame(IMainGame* parentGame) { m_game = parentGame; }
		void setRunning() { m_currentState = ScreenState::RUNNING; }
		//void setScreenIdx(int screenIdx) { m_screenIdx = screenIdx; }

		ScreenState getState() const { return m_currentState; }

	protected:
		int m_screenIdx = -1;
		IMainGame* m_game = nullptr;
		ScreenState m_currentState = ScreenState::NONE;
	};
}
