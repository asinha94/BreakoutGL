#pragma once
// std
#include <iostream>
#include "GameLevel.h"
// libs
#include <GLFW/glfw3.h>
//local


#define BOUT_OPENGL_MAJOR_VERSION 4
#define BOUT_OPENGL_MINOR_VERSION 3

const int DEFAULT_WIDTH  = 800;
const int DEFAULT_HEIGHT = 600;

enum GAME_STATE {
	GAME_ACTIVE = 1,
	GAME_MENU,
	GAME_WIN
};


namespace Bout {

	class Game {
	public:
		Game(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
		void Init();
		void ProcessInput(float);
		void Update();
		void Render();
		GLFWwindow* getWindow() { return m_window; }
		~Game();
	private:
		int m_width;
		int m_height;
		GLFWwindow* m_window;
		std::vector<GameLevel> Levels;
		unsigned int Level;
		int State;
		bool wait_for_release_d;
		bool wait_for_release_a;
	};
}