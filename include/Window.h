#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <iostream>
#include <GraphicsSystem.h>
#include <InputManager.h>

namespace engine {
	class GraphicsSystem;

	class Window {
	public:
		Window();
		~Window();

		GLFWwindow* CreateWindow(bool fullscreen = 0);

		InputManager input;
		Functionality functions;
	private:
		GraphicsSystem* m_graphics;
		GLFWwindow* appWindow;

		const GLFWvidmode* monitorInfo;
		GLFWmonitor* monitorContext;
	};
}