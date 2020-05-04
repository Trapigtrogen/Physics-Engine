#include <InputManager.h>

namespace engine {
	InputManager::InputManager() {
		keys.resize(0xFF);
	}
	InputManager::~InputManager() {}

	void InputManager::ProcessInput() {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
	}

	bool InputManager::GetKey(int keyCode) {
		return GetKeyState(keyCode) == HELD;
	}
	
	bool InputManager::GetKeyDown(int keyCode) {
		return GetKeyState(keyCode) == PRESSED;
	}
	
	bool InputManager::GetKeyUp(int keyCode) {
		return GetKeyState(keyCode) == RELEASED;
	}

	int InputManager::GetKeyState(int keyCode) {
		if (glfwGetKey(window, keyCode) == GLFW_RELEASE) {
			keys[keyCode] = RELEASED;
			return RELEASED;
		}
		keys[keyCode]++;
		if (keys[keyCode] > HELD) {
			keys[keyCode] = HELD;
		}
		return keys[keyCode];
	}

	void InputManager::SetWindow(GLFWwindow* appWindow) {
		window = appWindow;
	}
}