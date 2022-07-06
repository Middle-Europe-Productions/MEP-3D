#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"triangle.hpp"
#include"pyramid.hpp"

// Vertex Shader code
static const char* vShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
layout (location = 0) in vec3 pos;											  \n\
out vec4 local_color;                                                         \n\
                                                                              \n\
uniform mat4 model;                                                           \n\
void main()                                                                   \n\
{                                                                             \n\
    gl_Position = model * vec4(pos, 1.0);		                              \n\
    local_color = vec4(clamp(pos, 0.0f, 1.0f), 1.0);                          \n\
}";

// Fragment Shader
static const char* fShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
out vec4 colour;                                                              \n\
in vec4 local_color;                                                          \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    colour = local_color;                                                     \n\
}";

int main()
{
    if (!glfwInit()) {
        std::cout << "GLFW Error";
        glfwTerminate();
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(300, 300, "Test", NULL, NULL);
    if (!window) {
        std::cout << "GLFW window failed";
        glfwTerminate();
        return 1;
    }
    int buffer_X, buffer_Y;
    glfwGetFramebufferSize(window, &buffer_X, &buffer_Y);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "GLEW failed: " << glewGetErrorString(err);
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, buffer_X, buffer_Y);

    Pyramid a({
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
     });

     a.AddShader(vShader, GL_VERTEX_SHADER);
     a.AddShader(fShader, GL_FRAGMENT_SHADER);
     a.CompileShaders();
     //a.AddSpaceAction(std::make_unique<Scale>(0.5, 0.5, 1.0));
     //a.AddSpaceAction(std::make_unique<Transform>(1.0, 0.0, 0.0));
     //a.AddSpaceAction(std::make_unique<Rotate>(105, Axis::X));
     float curAngle = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        curAngle += 0.0001f;
        if (curAngle >= 360)
        {
            curAngle -= 360;
        }
        glfwPollEvents();
        a.AddSpaceAction(std::make_unique<Rotate>(curAngle, Axis::Y));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        a.Draw();
        glfwSwapBuffers(window);
    }
}