#pragma once
#include "Dengine.h"
#include "Window.h"
#include "InputManager.h"

#include <memory>

namespace Dengine {
    class ScreenList;
    class IGameScreen;

    class IMainGame
    {
    public:
        IMainGame();
        virtual ~IMainGame();

        void run();
        void exitGame();

        /// <summary>
        /// Used for Setting custom initialization behaviour
        /// in game programs by having game's main game class 
        /// override this behaviour
        /// </summary>
        virtual void onInit() = 0;

        /// <summary>
        /// Used for adding any screens game needs by overriding
        /// this in game program
        /// </summary>
        virtual void addScreens() = 0;

        /// <summary>
        /// Used for adding behaviour on exiting main game
        /// </summary>
        virtual void onExit() = 0;

        

        float getFPS() const { return m_fps; }

        void onSDLEvent(SDL_Event& evnt);

        InputManager inputManager;
        bool resizeCamera = false;

        int getWindowWidth() { return m_window.getScreenWidth(); }
        int getWindowHeight() { return m_window.getScreenHeight(); }

    protected:

        bool init();
        bool initSystems();

        virtual void update();
        virtual void draw();

        

        std::unique_ptr<ScreenList> m_screenList = nullptr;
        IGameScreen* m_currentScreen = nullptr;
        bool m_isRunning = false;
        float m_fps = 0.0f;
        

        Window m_window;
        
    };

}