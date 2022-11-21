// std
#include <iostream>
#include <functional>
// libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// game
#include "Game.h"
#include "GameObject.h"
#include "sprite.h"
#include "resource_manager.h"


namespace Bout {
	const glm::vec2 PLAYER_SIZE(100.f, 20.f);
	const float PLAYER_VELOCITY = 500.f;
	SpriteRenderer* renderer;
	GameObject* Player;

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	Game::Game(int width, int height) :
		m_width{ width },
		m_height{ height },
		m_window{ nullptr },
		Level{ 0 },
		State{ GAME_ACTIVE },
		wait_for_release_a{false},
		wait_for_release_d{false}
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, BOUT_OPENGL_MAJOR_VERSION);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, BOUT_OPENGL_MINOR_VERSION);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

		m_window = glfwCreateWindow(m_width, m_height, "Breakout", NULL, NULL);
		if (m_window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(m_window);
		glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}
	}

	void Game::Init()
	{
		auto shader = ResourceManager::LoadShader("sprite.vert", "sprite.frag", nullptr, "sprite");
		auto projection = glm::ortho(0.f, (float)this->m_width, (float)this->m_height, 0.f, -1.f, 1.f);
		
		shader.use();
		shader.SetInteger("image", 0);
		shader.SetMatrix4("projection", projection);
		
		renderer = new SpriteRenderer(shader);
		ResourceManager::LoadTexture("C:\\Projects\\BreakoutGL\\resources\\textures\\awesomeface.png", true, "face");
		ResourceManager::LoadTexture("C:\\Projects\\BreakoutGL\\resources\\textures\\background.jpg", false, "background");
		ResourceManager::LoadTexture("C:\\Projects\\BreakoutGL\\resources\\textures\\block.png", false, "block");
		ResourceManager::LoadTexture("C:\\Projects\\BreakoutGL\\resources\\textures\\block_solid.png", false, "block_solid");

		GameLevel one; one.Load("C:\\Projects\\BreakoutGL\\resources\\levels\\level1.txt", this->m_width, this->m_height / 2);
		GameLevel two; two.Load("C:\\Projects\\BreakoutGL\\resources\\levels\\level2.txt", this->m_width, this->m_height / 2);
		GameLevel three; three.Load("C:\\Projects\\BreakoutGL\\resources\\levels\\level3.txt", this->m_width, this->m_height / 2);
		GameLevel four; four.Load("C:\\Projects\\BreakoutGL\\resources\\levels\\level4.txt", this->m_width, this->m_height / 2);
		
		this->Levels.push_back(one);
		this->Levels.push_back(two);
		this->Levels.push_back(three);
		this->Levels.push_back(four);

		// Initialize the paddle bar
		ResourceManager::LoadTexture("C:\\Projects\\BreakoutGL\\resources\\textures\\paddle.png", true, "paddle");
		glm::vec2 playerPos = glm::vec2( ((m_width - PLAYER_SIZE.x) / 2.0f), m_height - PLAYER_SIZE.y);
		auto paddle = ResourceManager::GetTexture("paddle");
		Player = new GameObject(playerPos, PLAYER_SIZE, paddle);
	}
	void Game::ProcessInput(float dt)
	{
		if (State == GAME_ACTIVE) {
			float velocity = PLAYER_VELOCITY * dt;

			if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
				Player->Position.x -= velocity;
				if (Player->Position.x < 0)
					Player->Position.x = 0;
			}

			if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
				Player->Position.x += velocity;
				float end_pos = this->m_width - PLAYER_SIZE.x;
				if (Player->Position.x >= end_pos)
					Player->Position.x = end_pos;
			}
		}

		return;

		if (wait_for_release_d && glfwGetKey(m_window, GLFW_KEY_D) == GLFW_RELEASE) {
			wait_for_release_d = false;
			return;
		}

		if (wait_for_release_a && glfwGetKey(m_window, GLFW_KEY_A) == GLFW_RELEASE) {
			wait_for_release_a = false;
			return;
		}


		if (!wait_for_release_d && glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
			wait_for_release_d = true;
			if (Level < Levels.size() - 1)
				++Level;
			std::cout << Level << '\n';
			return;
		}

		if (!wait_for_release_a && glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
			wait_for_release_a = true;
			if (Level > 0)
				--Level;
			return;
		}
	}

	void Game::Update()
	{

	}

	void Game::Render()
	{
		if (this->State == GAME_ACTIVE) {
			// Draw the background
			auto background = ResourceManager::GetTexture("background");
			renderer->DrawSprite(
				background, glm::vec2(0.f, 0.f), glm::vec2(this->m_width, this->m_height), 0.f, glm::vec3(1.f, 1.f, 1.f)
			);

			// Draw the blocks
			Levels[Level].Draw(*renderer);
			Player->Draw(*renderer);
		}
		
	}

	Game::~Game()
	{
		delete renderer;
		delete Player;
		glfwTerminate();
	}

	

}