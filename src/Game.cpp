// std
#include <iostream>
// libs
#include <GLFW/glfw3.h>
#include <glad/glad.h>
// game
#include "Game.h"


namespace Bout {

	Game::Game(int width, int height) :
		m_width{ width },
		m_height{ height },
		m_window{ nullptr }
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, BOUT_OPENGL_MAJOR_VERSION);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, BOUT_OPENGL_MINOR_VERSION);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(m_width, m_height, "Breakout", NULL, NULL);
		if (m_window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(m_window);
		glfwSetFramebufferSizeCallback(m_window, (GLFWframebuffersizefun)framebuffer_size_callback);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}
	}

	void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		m_width = width;
		m_height = height;
		glViewport(0, 0, width, height);
	}

	Game::~Game()
	{
		glfwTerminate();
	}

}