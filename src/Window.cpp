#include <Window.h>

namespace engine {

	Window::Window() {}

	Window::~Window() {}

	GLFWwindow* Window::CreateWindow(bool fullscreen) {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		monitorContext = glfwGetPrimaryMonitor();
		monitorInfo = glfwGetVideoMode(monitorContext);
		float windowWidth = monitorInfo->width;
		float windowHeight = monitorInfo->height;

		// glfw window creation
		GLFWwindow* window = glfwCreateWindow(windowWidth / 1.5, windowHeight / 1.5, "Testailu", NULL, NULL);
		glfwSetWindowAspectRatio(window, 16, 9);
		if (window == NULL) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}
		if (fullscreen) {
			glfwSetWindowMonitor(window, monitorContext, 0, 0, windowWidth, windowHeight, monitorInfo->refreshRate);
		}

		glfwMakeContextCurrent(window);

		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
		}

		input.SetWindow(window);

		return window;
	}
}