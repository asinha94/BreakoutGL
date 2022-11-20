#pragma once
// std
#include <iostream>
// libs
#include <GLFW/glfw3.h>
//local


#define BOUT_OPENGL_MAJOR_VERSION 4
#define BOUT_OPENGL_MINOR_VERSION 3

const int DEFAULT_WIDTH  = 800;
const int DEFAULT_HEIGHT = 600;


namespace Bout {

	class Game {
	public:
		Game(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
		GLFWwindow* getWindow() { return m_window; }
		~Game();
	private:
		int m_width;
		int m_height;
		GLFWwindow* m_window;

		void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	};
}