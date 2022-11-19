#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include "stb_image.h"


// camera
static auto cameraFront = glm::vec3();
static float lastX = 800 / 2.0f;
static float lastY = 600 / 2.0f;
static bool firstMouse = true;
static float yaw = -90.0f;
static float pitch = 0.0f;

void processInput(GLFWwindow* window);

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // Offset the mouse value by the difference
    float x_offset = xpos - lastX;
    float y_offset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    if (pitch > 89.f)
        pitch = 89.f;
    if (pitch < -89.f)
        pitch = -89.f;

    auto direction = glm::vec3();
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(direction);
}

void outline(GLFWwindow* window)
{

    auto vertexShader = ogl::VertexShader("coord.vs");
    auto fullFragmentShader = ogl::FragmentShader("texture.fs");
    auto outlineFragmentShader = ogl::FragmentShader("single_color.fs");

    auto shaderProgram = ogl::ShaderProgram(vertexShader, fullFragmentShader);
    auto outlineShaderProgram = ogl::ShaderProgram(vertexShader, outlineFragmentShader);

    // Cube (not compatibable with face culling)
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    float cubeVertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
         // Bottom face
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
         // Top face
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
    };

    // Create VAO for Outlined boxes
    unsigned int vao1;
    glGenVertexArrays(1, &vao1);
    glBindVertexArray(vao1);

    // Bind 2D Texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int width, height, nrChannels;
    const char* texname = "C:\\Projects\\BreakoutGL\\resources\\textures\\container.jpg";
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texname, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load " << texname << '\n';
        return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    unsigned int vbo1;
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    // Set attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Need to set the texture coordinates (from the big vertices array)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Bind 2D Texture
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width2, height2, nrChannels2;
    const char* texname2 = "C:\\Projects\\BreakoutGL\\resources\\textures\\grass.png";
    unsigned char* data2 = stbi_load(texname2, &width2, &height2, &nrChannels2, 0);
    if (!data2) {
        std::cout << "Failed to load " << texname2 << '\n';
        return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data2);
    

    // Move camera back i.e push the object forward
    // auto view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -3.f));
    // Apply perspective to model. 2nd param is aspect ratio, 3rd is near plane, 4th is far plane
    auto projection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 100.f);

    int modelLoc = glGetUniformLocation(shaderProgram.m_id, "model");
    int viewLoc = glGetUniformLocation(shaderProgram.m_id, "view");
    int projectionLoc = glGetUniformLocation(shaderProgram.m_id, "projection");

    int modelLoc2 = glGetUniformLocation(outlineShaderProgram.m_id, "model");
    int viewLoc2 = glGetUniformLocation(outlineShaderProgram.m_id, "view");
    int projectionLoc2 = glGetUniformLocation(outlineShaderProgram.m_id, "projection");

    // Place cubes at these locations
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -12.5f)
    };

    auto grass_position = glm::vec3(0.f, -4.f, 0.f);

    // 3 units out (towards you) from the screen
    auto cameraPos = glm::vec3(0.f, 0.f, 3.f);
    //auto origin = glm::vec3(0.f, 0.f, 0.f);
    //auto cameraDirection = glm::normalize(cameraPos - origin);
    auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Don't need to do anything of the below if we use glm::LookAt
    //auto cameraRight = glm::vec3(1.0f, 0.0f, 0.0f); //glm::normalize(glm::cross(cameraFront, cameraUp));
    //auto cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    //auto cameraUp = glm::cross(cameraDirection, cameraRight);

    const float cameraSpeed = 0.05f;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    // what values we use when buffer values are cleared with  glClear(GL_COLOR_BUFFER_BIT)
    // i.e sets the background color
    glClearColor(0.5f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_STENCIL_TEST);
    
    // Enable culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Uncomment to change front-facing winding order
    //glFrontFace(GL_CW);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        glBindVertexArray(vao1);

        // render
        // ------
        glStencilMask(0xFF);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // make right always refer to camera-axis right, not world-axis (i.e positive x-axis) right
        auto cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraRight;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraRight;

        //float radius = 10.f;
        //float time = (float) glfwGetTime();
        //float camPosX = sin(time) * radius;
        //float camPosZ = cos(time) * radius;

        // camera stays grounded if uncommented
        //cameraPos.y = 0.f;
        auto view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glBindTexture(GL_TEXTURE_2D, texture2);
        // Draw everything but don't write to the stencil buffer
        glStencilMask(0x00);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        shaderProgram.use();
        auto vegetationModel = glm::translate(glm::mat4(1.f), grass_position);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(vegetationModel));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawArrays(GL_TRIANGLES, 6, 6);
        //glDrawArrays(GL_TRIANGLES, 12, 6);

        

        // Insert 10 rotating cubes in world space defined
        glBindTexture(GL_TEXTURE_2D, texture);
        float scale = 1.2f;
        for (int i = 0; i < 10; ++i) {
            shaderProgram.use();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

            // Create stencil based on all boxes rendered. Stencil only covers what is rendered
            glStencilMask(0xFF);
            glClear(GL_STENCIL_BUFFER_BIT);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glEnable(GL_DEPTH_TEST);

            auto cubePosition = cubePositions[i];
            auto model = glm::translate(glm::mat4(1.f), cubePosition);

            float angle = 20.f * i; //(float)glfwGetTime()* glm::radians(50.f);
            model = glm::rotate(model, angle, glm::vec3(1.f, 0.3f, 0.5f));

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Draw scaled outline
            outlineShaderProgram.use();
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);
            
            // Need to make sure depth testing is enabled here so boxes in the backround don't render behind our object
            //glDisable(GL_DEPTH_TEST);

            model = glm::scale(model, glm::vec3(scale, scale, scale));
            glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionLoc2, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }



        glBindVertexArray(0);

        // Only glStencilMask(0xFF); is necessary here, so that glClear can reset the buffer
        // But its good practice to reset the state to what it was before anyway
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}