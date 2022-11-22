// std
#include <iostream>
#include <functional>
#include <filesystem>
// libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// game
#include "Game.h"
#include "GameObject.h"
#include "sprite.h"
#include "resource_manager.h"
#include "Ball.h"
#include "Path.h"


namespace Bout {
	const glm::vec2 PLAYER_SIZE(100.f, 20.f);
	const float PLAYER_VELOCITY = 500.f;
	const glm::vec2 INITIAL_BALL_VELOCITY(100.f, -350.f);
	const float BALL_RADIUS = 12.5f;

	SpriteRenderer* renderer;
	GameObject* Player;
	BallObject* Ball;

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
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
		auto shader = ResourceManager::LoadShader(PATH("shaders/sprite.vert"), PATH("shaders/sprite.frag"), nullptr, "sprite");
		auto projection = glm::ortho(0.f, (float)this->m_width, (float)this->m_height, 0.f, -1.f, 1.f);
		
		shader.use();
		shader.SetInteger("image", 0);
		shader.SetMatrix4("projection", projection);
		
		renderer = new SpriteRenderer(shader);
		ResourceManager::LoadTexture(PATH("resources/textures/background.jpg"), false, "background");
		ResourceManager::LoadTexture(PATH("resources/textures/block.png"), false, "block");
		ResourceManager::LoadTexture(PATH("resources/textures/block_solid.png"), false, "block_solid");

		GameLevel one; one.Load(PATH("resources/levels/level1.txt"), this->m_width, this->m_height / 2);
		GameLevel two; two.Load(PATH("resources/levels/level2.txt"), this->m_width, this->m_height / 2);
		GameLevel three; three.Load(PATH("resources/levels/level3.txt"), this->m_width, this->m_height / 2);
		GameLevel four; four.Load(PATH("resources/levels/level4.txt"), this->m_width, this->m_height / 2);
		
		this->Levels.push_back(one);
		this->Levels.push_back(two);
		this->Levels.push_back(three);
		this->Levels.push_back(four);

		// Initialize the paddle bar
		ResourceManager::LoadTexture(PATH("resources/textures/paddle.png"), true, "paddle");
		glm::vec2 playerPos = glm::vec2( ((m_width - PLAYER_SIZE.x) / 2.0f), m_height - PLAYER_SIZE.y);
		auto paddle = ResourceManager::GetTexture("paddle");
		Player = new GameObject(playerPos, PLAYER_SIZE, paddle);

		ResourceManager::LoadTexture(PATH("resources/textures/awesomeface.png"), true, "face");
		auto ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
		auto face = ResourceManager::GetTexture("face");
		Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, face);
	}

	void Game::ProcessInput(float dt)
	{
		if (State == GAME_ACTIVE) {
			float velocity = PLAYER_VELOCITY * dt;

			if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
				
				if (Player->Position.x >= 0.f) {
					Player->Position.x -= velocity;
					if (Ball->Stuck)
						Ball->Position.x -= velocity;
				}
				
			}

			if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
				
				float end_pos = this->m_width - PLAYER_SIZE.x;
				if (Player->Position.x <= end_pos) {
					Player->Position.x += velocity;
					if (Ball->Stuck)
						Ball->Position.x += velocity;
				}
					
			}

			if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
				Ball->Stuck = false;
			}
		}
	}

	void Game::DoCollisions()
	{
		// Check collision with the paddle
		auto paddle_collision = Ball->CheckCollisionCircle(*Player);
		if (!Ball->Stuck && std::get<0>(paddle_collision))
		{
			// check where it hit the board, and change velocity based on where it hit the board
			float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
			float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
			float percentage = distance / (Player->Size.x / 2.0f);

			float strength = 2.0f;
			glm::vec2 oldVelocity = Ball->Velocity;
			Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;

			// Force the ball to go up if we collide
			Ball->Velocity.y = -1.f * abs(Ball->Velocity.y); //-Ball->Velocity.y;
			Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
			return;
		}

		for (GameObject& brick : this->Levels[this->Level].Bricks) {
			if (!brick.Destroyed) {
				auto collision = Ball->CheckCollisionCircle(brick);
				if (std::get<0>(collision))
				{
					if (!brick.IsSolid)
						brick.Destroyed = true;

					auto dir = std::get<1>(collision);
					auto diff_vector = std::get<2>(collision);

					if (dir == LEFT || dir == RIGHT) // Horizontal collision
					{
						Ball->Velocity.x = -Ball->Velocity.x;
						// relocate
						float penetration = Ball->Radius - std::abs(diff_vector.x);
						if (dir == LEFT)
							Ball->Position.x -= penetration; // move ball to left
						else
							Ball->Position.x += penetration; // move ball to right;
					}
					else { //Vertical collision
						Ball->Velocity.y = -Ball->Velocity.y; // reverse vertical velocity
						// relocate
						float penetration = Ball->Radius - std::abs(diff_vector.y);
						if (dir == UP)
							Ball->Position.y -= penetration; // move ball back up
						else
							Ball->Position.y += penetration; // move ball back down
					}
				}
			}
		}
	}

	void Game::Update(float dt)
	{
		Ball->Move(dt, this->m_width);
		DoCollisions();

		// Reset if ball dropped
		if (Ball->Position.y >= this->m_height)
		{
			this->ResetLevel();
			this->ResetPlayer();
		}
	}

	void Game::ResetLevel()
	{
		if (this->Level == 0)
			this->Levels[0].Load(PATH("resources/levels/level1.txt"), this->m_width, this->m_height / 2);
		else if (this->Level == 1)
			this->Levels[1].Load(PATH("resources/levels/level2.txt"), this->m_width, this->m_height / 2);
		else if (this->Level == 2)
			this->Levels[2].Load(PATH("resources/levels/level3.txt"), this->m_width, this->m_height / 2);
		else if (this->Level == 3)
			this->Levels[3].Load(PATH("resources/levels/level4.txt"), this->m_width, this->m_height / 2);
	}

	void Game::ResetPlayer()
	{
		// reset player/ball stats
		Player->Size = PLAYER_SIZE;
		Player->Position = glm::vec2(this->m_width / 2.0f - PLAYER_SIZE.x / 2.0f, this->m_height - PLAYER_SIZE.y);
		Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
	}

	void Game::Render()
	{
		if (this->State == GAME_ACTIVE) {
			// Draw the background
			auto background = ResourceManager::GetTexture("background");
			renderer->DrawSprite(
				background, glm::vec2(0.f, 0.f), glm::vec2(this->m_width, this->m_height), 0.f, glm::vec3(1.f, 1.f, 1.f)
			);

			// Draw the blocks, paddle and ball
			Levels[Level].Draw(*renderer);
			Player->Draw(*renderer);
			Ball->Draw(*renderer);
		}
		
	}

	Game::~Game()
	{
		delete renderer;
		delete Player;
		glfwTerminate();
	}

	

}
