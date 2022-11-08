#include <MEP-3D/mep-3d.hpp>
#include <MEP-3D/template/shaders_preset.hpp>


const char* kVertexShader = R"(
#version 330
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
  gl_Position = vec4(pos, 1.0);
}
)";

const char* kFragmentShader = R"(
#version 330
 
out vec4 color;

void main()
{
    color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";

class CameraScene final : public Scene {
 public:
  CameraScene() {}
  void OnAttach() override {
    // Load shader
    auto shader = std::make_unique<Shader>();
    triangle.Bind(shader.get());
    shader->CreateFromString(kVertexShader, kFragmentShader);
    shaders::CacheDefaultUnifroms(*shader);
    Attach(std::move(shader));
  }
  void OnDetach() override {}
  void OnUpdate(float time_delta) override {}
  void OnDraw(RenderTarget& render_target) override {
    GetShaders()[0]->Use();
    triangle.Draw(render_target);
    GetShaders()[0]->Stop();
  }
private:
  Triangle triangle;
};

int main(int argc, char* argv[]) {
  utils::Init(argc, argv);
  auto window = Window::GetInstance({{1280, 720}, "Camera example"});
  window->Init();
  auto engine = utils::CreateEngine(std::move(window),
                                    std::make_unique<CameraScene>());
  engine->SetClearColor(Color{0, 128, 0, 255});
  engine->Run();
}