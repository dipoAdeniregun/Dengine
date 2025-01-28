#pragma once
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <glm/glm.hpp>
#include <SDL/SDL_Events.h>

namespace Dengine {

	class GUI
	{
	public:
		void init(const std::string& resourceDir);
		void destroy();
		void draw();
		void update();

		static CEGUI::OpenGL3Renderer* getRenderer() { return m_renderer; }
		const CEGUI::GUIContext* getContext() { return m_context; }

		void loadScheme(const std::string& schemeFile);
		void setFont(const std::string& fontFile);
		CEGUI::Window* createWidget(const std::string& type, const glm::vec4& destRectPerc, 
									const glm::vec4& destRectPix, const std::string name = "");
		CEGUI::Window* createWidget(CEGUI::Window* parent, const std::string& type, const glm::vec4& destRectPerc,
			const glm::vec4& destRectPix, const std::string name = "");
		static void setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPerc, const glm::vec4& destRectPix);

		void setMouseCursor(const std::string& imageFile);
		void showMouseCursor();
		void hideMouseCursor();

		void onSDLEvent(SDL_Event& evnt);

	private:
		static CEGUI::OpenGL3Renderer* m_renderer;
		CEGUI::GUIContext* m_context = nullptr;
		CEGUI::Window* m_rootWindow = nullptr;
		unsigned int m_lastTime = 0;
		unsigned int m_elapsedTime = 0;
	};

}
