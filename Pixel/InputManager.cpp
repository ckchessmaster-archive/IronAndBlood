#include "InputManager.h"

namespace Pixel {

	InputManager::InputManager() : _mouseCoords(0.0f)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::pressKey(unsigned int keyID) {
		_keyMap[keyID] = true;
	}//end pressKey

	void InputManager::releaseKey(unsigned int keyID) {
		_keyMap[keyID] = false;
	}//end releaseKey

	void InputManager::setMouseCoords(float x, float y) {
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}//end setMouseCoords

	bool InputManager::isKeyPressed(unsigned int keyID) {
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end()) {
			return it->second;
		} else {
			return false;
		}//end if/else
	}//end isKeyPressed
}//end namespace