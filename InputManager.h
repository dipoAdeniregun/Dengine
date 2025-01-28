#pragma once
#include <unordered_map>
#include <glm/glm.hpp>

namespace Dengine {
	class InputManager
	{
	private:
		std::unordered_map<unsigned int, bool> keyMap;
		std::unordered_map<unsigned int, bool> previousKeyMap;

		glm::vec2 mouseCoords;
		glm::ivec2 mouseWheelScroll;

		bool resizeCamera_b = false;
		int numKeysPressed = 0;
		

	public:
		InputManager();
		~InputManager();

		bool mouseScrollConsumed = true;

		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);

		bool isKeyDown(unsigned int keyID);
		bool wasKeyDown(unsigned int keyID);

		bool isKeyPressed(unsigned int keyID);
		bool wasKeyReleased(unsigned int keyID);
		
		bool anyKeyPressed() { return numKeysPressed == 0; }
		void update();

		void setMouseCoords(float x, float y);
		void setMouseScrolled(int x, int y);
		glm::ivec2& getMouseScroll() { mouseScrollConsumed = true; return mouseWheelScroll; }

		void resizeCamera() { resizeCamera_b = true; }
		
		glm::vec2 getMouseCoords() const{ return mouseCoords; }
	};

}