#include<iostream>
#include<vector>

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"triangle.hpp"
#include"pyramid.hpp"
#include"shader.hpp"
#include"window.hpp"

class Game {
public:
    Game() {
        window_ = Window::GetInstance({ 1280, 720, "Example" });
        window_->Init();
    }
    void Init() {
        shader_.CreateFromFile("shaders/shader.vert", "shaders/shader.frag");
        std::vector<GLfloat> vec = {
            -1.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
        };
        pyramids.emplace_back(std::make_unique<Pyramid>(vec));
        pyramids.back()->Bind(&shader_);
        pyramids.emplace_back(std::make_unique<Pyramid>(vec));
        pyramids.back()->Bind(&shader_);
    }
    void RunUntilStopped() {
        float curAngle = 0;
        while (window_->IsOpen()) {
            curAngle += 0.0001f;
            if (curAngle >= 360)
            {
                curAngle -= 360;
            }
            glfwPollEvents();
            shader_.StartUsing();
            pyramids[0]->AddSpaceAction(std::make_unique<Rotate>(curAngle, Axis::Y));
            pyramids[1]->AddSpaceAction(std::make_unique<Rotate>(curAngle, Axis::Z));
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            pyramids[0]->Draw();
            pyramids[1]->Draw();
            shader_.StopUsing();
            window_->FinishLoop();
        }
    }
private:
    std::vector<std::unique_ptr<Pyramid>> pyramids;
    WindowPtr window_;
    Shader shader_;
};


int main()
{
    Game game;
    game.Init();
    game.RunUntilStopped();
}