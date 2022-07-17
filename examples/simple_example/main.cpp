#include<iostream>
#include<vector>

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<MEP-3D/pyramid.hpp>
#include<MEP-3D/shader.hpp>
#include<MEP-3D/window.hpp>
#include<MEP-3D/perspective_view.hpp>
#include<MEP-3D/window_observer.hpp>
#include<MEP-3D/image.hpp>
#include<MEP-3D/vector.hpp>
#include<MEP-3D/texture.hpp>
#include<MEP-3D/light.hpp>

class Game: private WindowObserver {
public:
    Game() {
        window_ = Window::GetInstance({ 800, 600, "Example" });
        window_->Init();
        view_ = std::make_unique<PerspectiveView>(PerspectiveView::Config({ glm::radians(45.0f),
            (GLfloat)window_->GetBufferWidth() / (GLfloat)window_->GetBufferHight(), 0.1f, 100.0f }));
        CameraConfig config = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 180.0f, 5.0f, 0.5f };
        camera_ = std::make_unique<Camera>(config);

        image.LoadFromFile("textures/dirt.png");
        tex = std::make_unique<Texture>();
        tex->Create(image);

        window_->AddView(view_.get());
        window_->AddCamera(camera_.get());
        window_->AddObserver(this);
        window_->AddObserver(camera_.get());
    }
    void Init() {
        shader_.CreateFromFile("shaders/shader.vert", "shaders/shader.frag");
        shader_.SaveUniformToMemory("projection", CommonUniform::Projection);
        shader_.SaveUniformToMemory("view", CommonUniform::View);
        shader_.SaveUniformToMemory("model", CommonUniform::Model);
        std::vector<GLfloat> vec = {
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 1.0f, 0.5f, 0.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.5f, 1.0f
        };
        light = std::make_unique<Light>(AmbientConfig{Color(255, 0, 255), 0.5f}, std::nullopt);
        light->BindUniform(shader_.GetUniform("directional_light.color"), LightUniforms::AmbientColor);
        light->BindUniform(shader_.GetUniform("directional_light.ambient_intensity"), LightUniforms::AmbientIntensity);
        pyramids.emplace_back(std::make_unique<Pyramid>(vec));
        pyramids.back()->Bind(&shader_);
        pyramids.back()->Bind(tex.get());
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
            glfwPollEvents();
            //LOG(INFO) << pyramids[0]->ToString();
            camera_->Update();
            shader_.StartUsing();
            light->Use();
            pyramids[0]->PushObjectAction(std::make_unique<Rotate>(0.01f, Axis::Y));
            //LOG(INFO) << pyramids[0]->ToString();
            //pyramids[1]->SetSpaceAction(std::make_unique<Rotate>(curAngle, Axis::Y));
            window_->Clear(White);
            LOG(INFO) << light->ToString();
            shader_.SetUniform("use_texture", 1);
            for (auto& x : pyramids) {
                x->Draw(*window_);
            }
            shader_.StopUsing();
            window_->FinishLoop();
        }
    }
    void OnKeyEvent(KeyEvent event) override {
        //LOG(INFO) << event.code << ", action: " << (event.action == Action::Pressed ? "Pressed" : "Released");
        if (event.code == Keyboard::Z) {
            pyramids[0]->PushObjectAction(std::make_unique<Rotate>(1.0f, Axis::Z));
            pyramids[0]->Update();
        }
        else if (event.code == Keyboard::R) {
            pyramids[0]->ResetModel();
        }
        else if (event.code == Keyboard::U) {
            if (tex) {
                tex = nullptr;
            }
        }
        else if (event.code == Keyboard::I) {
            if (!tex) {
                tex = std::make_unique<Texture>();
                tex->Create(image);
                pyramids[0]->Bind(tex.get());
            }
        }
        else if (event.code == Keyboard::O && event.action == Action::Released) {
            pyramids.pop_back();
        }
    }
    void OnMouseEvent(MouseEvent event) override {
        //LOG(INFO) << "Mouse event, x: " << event.x << ", y: " << event.y;
    }
    ~Game() {}
private:
    std::vector<std::unique_ptr<Pyramid>> pyramids;
    WindowPtr window_;
    std::unique_ptr<PerspectiveView> view_;
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<Texture> tex;
    Shader shader_;
    Shader shader_2;
    Image image;
    std::unique_ptr<Light> light;
    float curAngle_;
};


int main()
{
    Game game;
    game.Init();
    game.RunUntilStopped();
    return 1;
}