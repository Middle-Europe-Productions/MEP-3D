#include <iostream>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <MEP-3D/directional_light.hpp>
#include <MEP-3D/engine_pool.hpp>
#include <MEP-3D/figures.hpp>
#include <MEP-3D/image.hpp>
#include <MEP-3D/layer_assets.hpp>
#include <MEP-3D/light_controller.hpp>
#include <MEP-3D/material.hpp>
#include <MEP-3D/model.hpp>
#include <MEP-3D/perspective_view.hpp>
#include <MEP-3D/shader.hpp>
#include <MEP-3D/spot_light.hpp>
#include <MEP-3D/template/engine_data_ui_layer.hpp>
#include <MEP-3D/template/layer_assets_ui_layer.hpp>
#include <MEP-3D/texture.hpp>
#include <MEP-3D/user_interface.hpp>
#include <MEP-3D/utils.hpp>
#include <MEP-3D/vector.hpp>
#include <MEP-3D/window.hpp>
#include <MEP-3D/window_observer.hpp>

const std::unordered_map<LightUniforms, std::string>
    kDirectionalLightUniformMap = {
        {LightUniforms::AmbientColor, "directional_light.base.color"},
        {LightUniforms::AmbientIntensity,
         "directional_light.base.ambient_intensity"},
        {LightUniforms::DiffuseDirection, "directional_light.direction"},
        {LightUniforms::DiffuseIntensity,
         "directional_light.base.diffuse_intensity"}};

const std::unordered_map<MaterialUniform, std::string> kMaterialUniformMap = {
    {MaterialUniform::SpecularIntensity, "material.specular_intensity"},
    {MaterialUniform::Shininess, "material.shininess"}};

class BenchmarkLayer final : public Layer, public LayerAssets {
 public:
  BenchmarkLayer(unsigned int triangles_count)
      : Layer("benchmark_layer_" + std::to_string(triangles_count)),
        triangles_count_(triangles_count) {}
  void OnAttach() override {
    auto& window = GetEngine()->GetWindow();
    // Camera config
    view_ = std::make_unique<PerspectiveView>(PerspectiveView::Config(
        {glm::radians(45.0f), window->GetAspectRation(), 0.1f, 10000.0f}));
    CameraConfig config = {glm::vec3(0.0f, 0.0f, 0.0f),
                           glm::vec3(0.0f, 1.0f, 0.0f),
                           -90.0f,
                           180.0f,
                           5.0f,
                           0.5f};
    camera_ = std::make_unique<Camera>(config);
    // Load texture
    image.LoadFromFile("plain.png");
    plain_tex = std::make_unique<Texture>();
    plain_tex->Create(image);
    // Init window
    window->AddView(view_.get());
    window->AddCamera(camera_.get());
    window->AddObserver(camera_.get());
    // Load shader
    shader_.CreateFromFile("shaders/shader.vert", "shaders/shader.frag");
    shader_.SaveUniformToMemory(
        "projection", static_cast<unsigned int>(CommonUniform::Projection));
    shader_.SaveUniformToMemory("view",
                                static_cast<unsigned int>(CommonUniform::View));
    shader_.SaveUniformToMemory(
        "model", static_cast<unsigned int>(CommonUniform::Model));
    shader_.SaveUniformToMemory(
        "eye_position", static_cast<unsigned int>(CommonUniform::Position));
    light = std::make_unique<DirectionalLight>(
        AmbientConfig{Color(255, 255, 255), 0.1f},
        DiffuseConfig{Vec3f(-2.0f, -1.0f, -2.0f), 1.0f});
    light->BindUniforms(shader_, kDirectionalLightUniformMap);

    AttachDirectionaLight(std::move(light));
    // Add UI
    auto ui_layer = LayerAssetsUILayer::Create();
    LayerAssets::AddObserver(ui_layer.get());
    GetEngine()->AttachLayerToStructure(std::move(ui_layer), 0);
    // Create elements
    for (int i = 0; i < triangles_count_; i++) {
      triangles_.emplace_back(std::make_unique<Pyramid>(
          Vec3f{static_cast<float>(triangles_count_ % 10 * 10.f), 1.0f,
                static_cast<float>(i + 10)}));
      triangles_.back()->Bind(plain_tex.get());
      triangles_.back()->Bind(&shader_);
    }
    // Init plane
    plane = std::make_unique<Plane>(100.0f);
  }
  void OnDetach() override {}
  void OnUpdate(float time_delta) { camera_->Update(); }
  void OnDraw(RenderTarget& render_target) {
    shader_.StartUsing();
    UseAllDirectionalLights();
    plain_tex->Use();
    shader_.SetUniform("use_texture", 1);
    for (auto& tr : triangles_) {
      tr->Draw(render_target);
    }
    plane->Draw(render_target);
    shader_.StopUsing();
  }

 private:
  std::vector<std::unique_ptr<Pyramid>> triangles_;
  Shader shader_;
  unsigned int triangles_count_;
  Image image;
  std::unique_ptr<Plane> plane;
  std::unique_ptr<Texture> plain_tex;
  std::unique_ptr<Camera> camera_;
  std::unique_ptr<PerspectiveView> view_;
  std::unique_ptr<DirectionalLight> light;
};

int main() {
  auto window = Window::GetInstance({{1280, 720}, "ImGui Example"});
  window->Init();
  auto main_engine = std::make_shared<Engine>();

  auto engine_data = EngineDataUILayer::Create();
  std::unique_ptr<Engine::CustomLayerStructure> custom_structure =
      std::make_unique<Engine::CustomLayerStructure>();
  window->AddObserver(engine_data.get());
  custom_structure->structure_name = std::string("ImGui"),
  custom_structure->before_run = UI::BeforeRender;
  custom_structure->after_run = UI::AfterRender;
  custom_structure->layer_array.push_back(std::move(engine_data));

  main_engine->AttachStructure(std::move(custom_structure));
  main_engine->AttachWindow(std::move(window));
  std::unique_ptr<Layer> master_layer = std::make_unique<BenchmarkLayer>(10000);
  main_engine->AttachLayer(std::move(master_layer));

  EnginePool::AttachEngineAndExecuteTask(std::move(main_engine), ENGINE_RUN);
}