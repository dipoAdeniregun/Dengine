#include "InputManager.h"


namespace Dengine {
	InputManager::InputManager():mouseCoords(0.0f), mouseWheelScroll(0.0f)
	{}
	InputManager::~InputManager(){}

	//press key
	void InputManager::pressKey(unsigned int keyID) {
		keyMap[keyID] = true;
		numKeysPressed++;
	}

	//relase key
	void InputManager::releaseKey(unsigned int keyID) {
		keyMap[keyID] = false;
		numKeysPressed--;
	}
	
	//check if a key is held down. return true if yes
	bool InputManager::isKeyDown(unsigned int keyID) {
		auto it = keyMap.find(keyID);
		if (it != keyMap.end()) {
			return it->second;
		}
		return false;
	}

	//check if key was down in previous frame
	bool InputManager::wasKeyDown(unsigned int keyID) {
		auto it = previousKeyMap.find(keyID);
		if (it != previousKeyMap.end()) {
			return it->second;
		}
		return false;
	}

	//check if a key was just pressed this frame and not pressed in the previous frame
	bool InputManager::isKeyPressed(unsigned int keyID) {
		return isKeyDown(keyID) && !(wasKeyDown(keyID)) ;
	}

	//check if a key was pressed in the previous frame but not this one(a keyup)
	bool InputManager::wasKeyReleased(unsigned int keyID)
	{
		return wasKeyDown(keyID) && !(isKeyDown(keyID));
	}

	void InputManager::update() {
		//copy all contents of current key map into previousKeyMap
		for (auto& it : keyMap) {
			previousKeyMap[it.first] = it.second;
		}
		
	}

	//set mouse coordinates using global coordinate system
	void InputManager::setMouseCoords(float x, float y) {
		mouseCoords.x = x;
		mouseCoords.y = y;
	}

	void InputManager::setMouseScrolled(int x, int y)
	{
		mouseWheelScroll.x = x;
		mouseWheelScroll.y = y;
		mouseScrollConsumed = false;
	}

}
