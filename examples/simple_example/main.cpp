#include<iostream>
#include<vector>

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<MEP-3D/triangle.hpp>
#include<MEP-3D/pyramid.hpp>
#include<MEP-3D/shader.hpp>
#include<MEP-3D/window.hpp>
#include<MEP-3D/three_dim_view.hpp>
#include<MEP-3D/window_observer.hpp>
#include<MEP-3D/image.hpp>

class Game: private WindowObserver {
public:
    Game() {
        Image image;
        image.LoadFromFile("tetures/texture.png");
        window_ = Window::GetInstance({ 800, 600, "Example" });
        view_ = std::make_unique<ThreeDimView>(ThreeDimView::Config({ glm::radians(45.0f),
            (GLfloat)window_->GetBufferWidth() / (GLfloat)window_->GetBufferHight(), 0.1f, 100.0f }));
        CameraConfig config = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 180.0f, 5.0f, 0.5f };
        camera_ = std::make_unique<Camera>(config);

        window_->Init();
        window_->AddView(view_.get());
        window_->AddCamera(camera_.get());
        window_->AddObserver(this);
        window_->AddObserver(camera_.get());
    }
    void Init() {
        shader_.CreateFromFile("shader.vert", "shader.frag");
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
        pyramids[0]->PushObjectAction(std::make_unique<Transform>(0.0f, 1.0f, 0.0f));
        pyramids[1]->PushObjectAction(std::make_unique<Transform>(0.0f, -1.0f, 0.0f));
        while (window_->IsOpen()) {
            curAngle_ += 0.0001f;
            if (curAngle_ >= 360)
            {
                curAngle_ -= 360;
            }
            //LOG(INFO) << camera_->ToString();
            glfwPollEvents();
            camera_->Update();
            shader_.StartUsing();
            pyramids[0]->PushObjectAction(std::make_unique<Rotate>(0.01f, Axis::Y));
            LOG(INFO) << pyramids[0]->ToString();
           // pyramids[1]->SetSpaceAction(std::make_unique<Rotate>(curAngle, Axis::Y));
            window_->Clear();
            pyramids[0]->Draw(*window_);
            pyramids[1]->Draw(*window_);
            shader_.StopUsing();
            window_->FinishLoop();
        }
    }
    void OnKeyEvent(KeyEvent event) override {
        LOG(INFO) << event.code << ", action: " << (event.action == Action::Pressed ? "Pressed" : "Released");
        if (event.code == Keyboard::Z) {
            pyramids[0]->PushObjectAction(std::make_unique<Rotate>(1.0f, Axis::Z));
            pyramids[0]->Update();
        }
        else if (event.code == Keyboard::R)
        {
            pyramids[0]->ResetModel();
        }
    }
    void OnMouseEvent(MouseEvent event) override {
        LOG(INFO) << "Mouse event, x: " << event.x << ", y: " << event.y;
    }
private:
    std::vector<std::unique_ptr<Pyramid>> pyramids;
    WindowPtr window_;
    std::unique_ptr<ThreeDimView> view_;
    std::unique_ptr<Camera> camera_;
    Shader shader_;
    float curAngle_;
};


int main()
{
    Game game;
    game.Init();
    game.RunUntilStopped();
}