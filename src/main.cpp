#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"triangle.hpp"
#include"pyramid.hpp"
#include"shader.hpp"
#include"window.hpp"

int main()
{
    auto window = Window::GetInstance({ 1280, 720, "Example" });
    window->Init();

    Shader shader;
    shader.CreateFromFile("shaders/shader.vert", "shaders/shader.frag");

    Pyramid a({
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    });
    Pyramid b({
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    });
    a.Bind(&shader);
    b.Bind(&shader);
    //a.AddSpaceAction(std::make_unique<Scale>(0.5, 0.5, 1.0));
    //a.AddSpaceAction(std::make_unique<Transform>(1.0, 0.0, 0.0));
    //a.AddSpaceAction(std::make_unique<Rotate>(105, Axis::X));
    float curAngle = 0.0f;
    while (window->IsOpen()) {
        curAngle += 0.0001f;
        if (curAngle >= 360)
        {
            curAngle -= 360;
        }
        glfwPollEvents();
        shader.StartUsing();
        a.AddSpaceAction(std::make_unique<Rotate>(curAngle, Axis::Y));
        b.AddSpaceAction(std::make_unique<Rotate>(curAngle, Axis::Z));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        a.Draw();
        b.Draw();
        shader.StopUsing();
        window->FinishLoop();
    }
}