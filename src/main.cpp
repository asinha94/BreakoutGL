#define STB_IMAGE_IMPLEMENTATION

// std
#include <iostream>
// libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
// Game
#include "Game.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void outline(GLFWwindow* window);
void fb(GLFWwindow* window);
void skybox(GLFWwindow* window);
void instance(GLFWwindow* window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    auto game = Bout::Game{};
    auto window = game.getWindow();

    //outline(window);
    //fb(window);
    skybox(window);
    //instance(window);


    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}