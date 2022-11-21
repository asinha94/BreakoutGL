#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "texture.h"


namespace Bout {
	class SpriteRenderer {
	public:
		SpriteRenderer(ShaderProgram& shader) : shader{ shader }, quadVAO{0} {
			
			float vertices[] = {
				0.f, 1.f, 0.f, 1.f,
				1.f, 0.f, 1.f, 0.f,
				0.f, 0.f, 0.f, 0.f,

				0.f, 1.f, 0.f, 1.f,
				1.f, 1.f, 1.f, 1.f,
				1.f, 0.f, 1.f, 0.f
			};

			glGenVertexArrays(1, &this->quadVAO);
			glBindVertexArray(this->quadVAO);
			
			unsigned int VBO;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FLOAT, 4 * sizeof(float), (void*)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		void DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
		{
			this->shader.use();

			// Identity matrix
			auto model = glm::mat4(1.f);
			// translate the model
			model = glm::translate(glm::mat4(1.f), glm::vec3(position, 0.f));

			// OpenGL coordinates has the origin at the center of the screen with range -1 to 1.
			// Our world space is from 0-800, 0-600, with the origin at the top left corner.
			// The conversion of those coordinates takes place later (with the orthographic transformation)
			// But our sprite here has its quad in the top right quadrant
			// so we first need to translate it down to the center, then rotate, then translate back
			model = glm::translate(model, glm::vec3(0.5f * size, 0.0f));
			model = glm::rotate(model, glm::radians(rotate),  glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.5f * size, 0.0f));
			// In linear algebra land, matrices are computed right to left, but the meaning is still left to right
			// i.e in the above we are translating by size to the left (-ive direction), rotating, then moving back

			model = glm::scale(model, glm::vec3(size, 1.0f));

			this->shader.SetMatrix4("model", model);
			this->shader.SetVector3f("spriteColor", color);

			glActiveTexture(GL_TEXTURE0);
			texture.Bind();

			glBindVertexArray(this->quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

		~SpriteRenderer()
		{
			glDeleteVertexArrays(1, &this->quadVAO);
		}
	private:
		ShaderProgram shader;
		unsigned int quadVAO;
	};


}
