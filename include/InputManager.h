#include <GLFW/glfw3.h> 
#include <vector>

namespace engine {
	class InputManager {
	public:
		InputManager();
		~InputManager();

		enum KeyState {
			RELEASED = 0,
			PRESSED = 1,
			HELD = 2
		};

		void ProcessInput();
		bool GetKeyDown(int keyCode);
		bool GetKeyUp(int keyCode);
		bool GetKey(int keyCode);

		void SetWindow(GLFWwindow* appWindow);
	private:
		GLFWwindow* window;
		std::vector<int> keys;

		int GetKeyState(int keyCode);
	};
}