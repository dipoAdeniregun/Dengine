#include "GUI.h"

#include <SDL/SDL_Timer.h>

namespace Dengine {
	CEGUI::OpenGL3Renderer* GUI::m_renderer = nullptr;
	void GUI::init(const std::string& resourceDir)
	{
		//check if renderer was aleady initialized
		if (m_renderer == nullptr) {
			m_renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();

			CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>( CEGUI::System::getSingleton().getResourceProvider());
			
			rp->setResourceGroupDirectory("imagesets", resourceDir + "/imagesets/");
			rp->setResourceGroupDirectory("schemes", resourceDir + "/schemes/");
			rp->setResourceGroupDirectory("fonts", resourceDir + "/fonts/");
			rp->setResourceGroupDirectory("layouts", resourceDir + "/layouts/");
			rp->setResourceGroupDirectory("looknfeels", resourceDir + "/looknfeel/");
			rp->setResourceGroupDirectory("lua_scripts", resourceDir + "/lua_scripts/");

			CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
			CEGUI::Scheme::setDefaultResourceGroup("schemes");
			CEGUI::Font::setDefaultResourceGroup("fonts");
			CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
			CEGUI::WindowManager::setDefaultResourceGroup("layouts");
			CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
		}

		m_context = &(CEGUI::System::getSingleton().createGUIContext(m_renderer->getDefaultRenderTarget()));
		m_rootWindow = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
		m_context->setRootWindow(m_rootWindow);
	}

	void GUI::destroy()
	{
		CEGUI::System::getSingleton().destroyGUIContext(*m_context);
	}

	void GUI::draw()
	{
		glDisable(GL_DEPTH_TEST);
		m_renderer->beginRendering();
		m_context->draw();
		m_renderer->endRendering();
		// Clean up after CEGUI
		glBindVertexArray(0);
		glDisable(GL_SCISSOR_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

    void GUI::update()
    {
        if (m_lastTime == 0) {
            m_elapsedTime = 0;
            m_lastTime = SDL_GetTicks();
        }
        else {
            unsigned int nextTime = SDL_GetTicks();
            m_elapsedTime = nextTime - m_lastTime;
            m_lastTime = nextTime;
        }
        m_context->injectTimePulse((float)m_elapsedTime / 1000.0f);
    }

	void GUI::loadScheme(const std::string& schemeFile)
	{
		CEGUI::SchemeManager::getSingleton().createFromFile(schemeFile);
	}

	void GUI::setFont(const std::string& fontFile)
	{
		CEGUI::FontManager::getSingleton().createFromFile(fontFile + ".font");
		m_context->setDefaultFont(fontFile);
	}

	CEGUI::Window* GUI::createWidget(const std::string& type, const glm::vec4& destRectPerc,
									 const glm::vec4& destRectPix, const std::string name /* = ""*/)
	{
		CEGUI::Window* newChild = CEGUI::WindowManager::getSingleton().createWindow(type, name);
		m_rootWindow->addChild(newChild);
		GUI::setWidgetDestRect(newChild, destRectPerc, destRectPix);
		return newChild;
	}

    CEGUI::Window* GUI::createWidget(CEGUI::Window* parent, const std::string& type, const glm::vec4& destRectPerc, const glm::vec4& destRectPix, const std::string name)
    {
        CEGUI::Window* newChild = CEGUI::WindowManager::getSingleton().createWindow(type, name);
        parent->addChild(newChild);
        GUI::setWidgetDestRect(newChild, destRectPerc, destRectPix);
        return newChild;
    }

	void GUI::setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPerc, const glm::vec4& destRectPix)
	{
		widget->setPosition(CEGUI::UVector2(CEGUI::UDim(destRectPerc.x, destRectPix.x), CEGUI::UDim(destRectPerc.y, destRectPix.y)));
		widget->setSize(CEGUI::USize(CEGUI::UDim(destRectPerc.z, destRectPix.z), CEGUI::UDim(destRectPerc.w, destRectPix.w)));
	}

	void GUI::setMouseCursor(const std::string& imageFile)
	{
		m_context->getMouseCursor().setDefaultImage(imageFile);
	}

	void GUI::showMouseCursor()
	{
		m_context->getMouseCursor().show();
	}

	void GUI::hideMouseCursor()
	{
		m_context->getMouseCursor().hide();
	}

    /************************************************************************
     Translate a SDLKey to the proper CEGUI::Key
 *************************************************************************/
    CEGUI::Key::Scan SDLKeyToCEGUIKey(SDL_Keycode key)
    {
        using namespace CEGUI;
        switch (key)
        {
            case SDLK_BACKSPACE:    return Key::Backspace;
            case SDLK_TAB:          return Key::Tab;
            case SDLK_RETURN:       return Key::Return;
            case SDLK_PAUSE:        return Key::Pause;
            case SDLK_ESCAPE:       return Key::Escape;
            case SDLK_SPACE:        return Key::Space;
            case SDLK_COMMA:        return Key::Comma;
            case SDLK_MINUS:        return Key::Minus;
            case SDLK_PERIOD:       return Key::Period;
            case SDLK_SLASH:        return Key::Slash;
            case SDLK_0:            return Key::Zero;
            case SDLK_1:            return Key::One;
            case SDLK_2:            return Key::Two;
            case SDLK_3:            return Key::Three;
            case SDLK_4:            return Key::Four;
            case SDLK_5:            return Key::Five;
            case SDLK_6:            return Key::Six;
            case SDLK_7:            return Key::Seven;
            case SDLK_8:            return Key::Eight;
            case SDLK_9:            return Key::Nine;
            case SDLK_COLON:        return Key::Colon;
            case SDLK_SEMICOLON:    return Key::Semicolon;
            case SDLK_EQUALS:       return Key::Equals;
            case SDLK_LEFTBRACKET:  return Key::LeftBracket;
            case SDLK_BACKSLASH:    return Key::Backslash;
            case SDLK_RIGHTBRACKET: return Key::RightBracket;
            case SDLK_a:            return Key::A;
            case SDLK_b:            return Key::B;
            case SDLK_c:            return Key::C;
            case SDLK_d:            return Key::D;
            case SDLK_e:            return Key::E;
            case SDLK_f:            return Key::F;
            case SDLK_g:            return Key::G;
            case SDLK_h:            return Key::H;
            case SDLK_i:            return Key::I;
            case SDLK_j:            return Key::J;
            case SDLK_k:            return Key::K;
            case SDLK_l:            return Key::L;
            case SDLK_m:            return Key::M;
            case SDLK_n:            return Key::N;
            case SDLK_o:            return Key::O;
            case SDLK_p:            return Key::P;
            case SDLK_q:            return Key::Q;
            case SDLK_r:            return Key::R;
            case SDLK_s:            return Key::S;
            case SDLK_t:            return Key::T;
            case SDLK_u:            return Key::U;
            case SDLK_v:            return Key::V;
            case SDLK_w:            return Key::W;
            case SDLK_x:            return Key::X;
            case SDLK_y:            return Key::Y;
            case SDLK_z:            return Key::Z;
            case SDLK_DELETE:       return Key::Delete;
            case SDLK_KP_0:          return Key::Numpad0;
            case SDLK_KP_1:          return Key::Numpad1;
            case SDLK_KP_2:          return Key::Numpad2;
            case SDLK_KP_3:          return Key::Numpad3;
            case SDLK_KP_4:          return Key::Numpad4;
            case SDLK_KP_5:          return Key::Numpad5;
            case SDLK_KP_6:          return Key::Numpad6;
            case SDLK_KP_7:          return Key::Numpad7;
            case SDLK_KP_8:          return Key::Numpad8;
            case SDLK_KP_9:          return Key::Numpad9;
            case SDLK_KP_PERIOD:    return Key::Decimal;
            case SDLK_KP_DIVIDE:    return Key::Divide;
            case SDLK_KP_MULTIPLY:  return Key::Multiply;
            case SDLK_KP_MINUS:     return Key::Subtract;
            case SDLK_KP_PLUS:      return Key::Add;
            case SDLK_KP_ENTER:     return Key::NumpadEnter;
            case SDLK_KP_EQUALS:    return Key::NumpadEquals;
            case SDLK_UP:           return Key::ArrowUp;
            case SDLK_DOWN:         return Key::ArrowDown;
            case SDLK_RIGHT:        return Key::ArrowRight;
            case SDLK_LEFT:         return Key::ArrowLeft;
            case SDLK_INSERT:       return Key::Insert;
            case SDLK_HOME:         return Key::Home;
            case SDLK_END:          return Key::End;
            case SDLK_PAGEUP:       return Key::PageUp;
            case SDLK_PAGEDOWN:     return Key::PageDown;
            case SDLK_F1:           return Key::F1;
            case SDLK_F2:           return Key::F2;
            case SDLK_F3:           return Key::F3;
            case SDLK_F4:           return Key::F4;
            case SDLK_F5:           return Key::F5;
            case SDLK_F6:           return Key::F6;
            case SDLK_F7:           return Key::F7;
            case SDLK_F8:           return Key::F8;
            case SDLK_F9:           return Key::F9;
            case SDLK_F10:          return Key::F10;
            case SDLK_F11:          return Key::F11;
            case SDLK_F12:          return Key::F12;
            case SDLK_F13:          return Key::F13;
            case SDLK_F14:          return Key::F14;
            case SDLK_F15:          return Key::F15;
            case SDLK_NUMLOCKCLEAR:      return Key::NumLock;
            case SDLK_SCROLLLOCK:    return Key::ScrollLock;
            case SDLK_RSHIFT:       return Key::RightShift;
            case SDLK_LSHIFT:       return Key::LeftShift;
            case SDLK_RCTRL:        return Key::RightControl;
            case SDLK_LCTRL:        return Key::LeftControl;
            case SDLK_RALT:         return Key::RightAlt;
            case SDLK_LALT:         return Key::LeftAlt;
        
            case SDLK_SYSREQ:       return Key::SysRq;
            case SDLK_MENU:         return Key::AppMenu;
            case SDLK_POWER:        return Key::Power;
            default:                return Key::Unknown;
        }
    }

    CEGUI::MouseButton SDLButtonToCEGUIButton(Uint8 mouseButton) {
        switch (mouseButton) {
            case SDL_BUTTON_LEFT:
                return CEGUI::MouseButton::LeftButton;
                break;
            case SDL_BUTTON_RIGHT:
                return CEGUI::MouseButton::RightButton;
                break;
            case SDL_BUTTON_MIDDLE:
                return CEGUI::MouseButton::MiddleButton;
                break;
            case SDL_BUTTON_X1:
                return CEGUI::MouseButton::X1Button;
                break;
            case SDL_BUTTON_X2:
                return CEGUI::MouseButton::X2Button;
                break;
            default:
                return CEGUI::MouseButton::NoButton;
                break;
        }
    }

	void GUI::onSDLEvent(SDL_Event& evnt)
	{
        CEGUI::utf32 codePoint;

		switch (evnt.type) {
			case SDL_MOUSEMOTION:
				m_context->injectMousePosition(evnt.motion.x, evnt.motion.y);
				break;
			case SDL_KEYDOWN:
                m_context->injectKeyDown(SDLKeyToCEGUIKey(evnt.key.keysym.sym));
				break;
			case SDL_KEYUP:
                m_context->injectKeyUp(SDLKeyToCEGUIKey(evnt.key.keysym.sym));
				break;
			case SDL_TEXTINPUT:
                codePoint = 0;
                for (int i = 0; evnt.text.text[i] != '\0'; i++) {
                    codePoint |= ((CEGUI::utf32)evnt.text.text[i]) << (i * 8);
                }
                m_context->injectChar(codePoint);
				break;
			case SDL_MOUSEBUTTONDOWN:
                m_context->injectMouseButtonDown(SDLButtonToCEGUIButton(evnt.button.button));
				break;
			case SDL_MOUSEBUTTONUP:
                m_context->injectMouseButtonUp(SDLButtonToCEGUIButton(evnt.button.button));
				break;
		}
	}
}
