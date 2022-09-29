#include <iostream>
#include <vector>

#include <MEP-3D/mep-3d.hpp>

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

const std::unordered_map<LightUniforms, std::string> kPointLightUniformMap = {
    {LightUniforms::AmbientColor, "base.color"},
    {LightUniforms::AmbientIntensity, "base.ambient_intensity"},
    {LightUniforms::DiffuseIntensity, "base.diffuse_intensity"},
    {LightUniforms::Position, "position"},
    {LightUniforms::Constant, "constant"},
    {LightUniforms::Linear, "linear"},
    {LightUniforms::Quadratic, "quadratic"}};

const std::unordered_map<LightUniforms, std::string> kSpotLightUniformMap = {
    {LightUniforms::AmbientColor, "base.base.color"},
    {LightUniforms::AmbientIntensity, "base.base.ambient_intensity"},
    {LightUniforms::DiffuseIntensity, "base.base.diffuse_intensity"},
    {LightUniforms::Position, "base.position"},
    {LightUniforms::Constant, "base.constant"},
    {LightUniforms::Linear, "base.linear"},
    {LightUniforms::Quadratic, "base.quadratic"},
    {LightUniforms::Direction, "direction"},
    {LightUniforms::Edge, "edge"}};

class BenchmarkLayer final : public Scene, private WindowObserver {
 public:
  BenchmarkLayer(unsigned int triangles_count)
      : Scene("benchmark_layer_" + std::to_string(triangles_count)),
        triangles_count_(triangles_count),
        shader_(std::make_unique<Shader>("main_shader")) {}
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
    window->AddObserver(this);
    // Load shader
    shader_->CreateFromFile("shaders/shader.vert", "shaders/shader.frag");
    shader_->Cache("projection",
                   static_cast<unsigned int>(CommonUniform::Projection));
    shader_->Cache("view", static_cast<unsigned int>(CommonUniform::View));
    shader_->Cache("model", static_cast<unsigned int>(CommonUniform::Model));
    shader_->Cache("eye_position",
                   static_cast<unsigned int>(CommonUniform::Position));
    // Directional light
    light = std::make_unique<DirectionalLight>(
        AmbientConfig{Color(255, 255, 255), 0.1f},
        DiffuseConfig{Vec3f(-2.0f, -1.0f, -2.0f), 1.0f});
    light->BindUniforms(*shader_, kDirectionalLightUniformMap);
    // Point lights
    point_light_con = std::make_unique<PointLightController>(
        16, "point_light", shader_->GetUniform("point_light_count"),
        kPointLightUniformMap);
    point_light_con->Bind(shader_.get());
    point_light_con->MakeAndBind(std::make_unique<PointLight>(
        AmbientConfig{Color(0, 247, 255), 0.5f},
        PointConfig{Vec3f{1.0f, 1.0f, 1.0f}, 1, 1, 0.001}, 0.5f));

    AttachDirectionaLight(std::move(light));
    AttachPointLightController(std::move(point_light_con));

    auto spot_light_con = std::make_unique<SpotLightController>(
        16, "spot_light", shader_->GetUniform("spot_light_count"),
        kSpotLightUniformMap);
    spot_light_con->Bind(shader_.get());
    AttachSpotLightController(std::move(spot_light_con));
    // Add UI
    auto ui_layer = SceneUILayer::Create();
    Scene::AddObserver(ui_layer.get());
    GetEngine()->AttachLayerToStructure(std::move(ui_layer), 0, true);
    // Create elements
    for (int i = 0; i < triangles_count_; i++) {
      triangles_.emplace_back(std::make_unique<Pyramid>(
          Vec3f{static_cast<float>(triangles_count_ % 10 * 10.f), 1.0f,
                static_cast<float>(i + 10)}));
      triangles_.back()->Bind(plain_tex.get());
      triangles_.back()->Bind(shader_.get());
    }
    // Init plane
    plane = std::make_unique<Plane>(100.0f);
    plane->Bind(plain_tex.get());
    plane->Bind(shader_.get());
    AttachShader(std::move(shader_));
    AttachTexture(std::move(plain_tex));
  }
  void OnDetach() override {}
  void OnUpdate(float time_delta) override { camera_->Update(); }
  void OnDraw(RenderTarget& render_target) override {
    GetShaders()[0]->Use();
    UseAllLights();
    DrawAllModels(render_target);
    GetTexture()[0]->Use();
    GetShaders()[0]->SetUniform("use_texture", 1);
    for (auto& tr : triangles_) {
      tr->Draw(render_target);
    }
    plane->Draw(render_target);
    GetShaders()[0]->Stop();
  }
  void OnKeyEvent(KeyEvent event) override {}
  void OnMouseEvent(MouseEvent event) override {}
  void OnWindowResizeEvent(Vec2i size) override {
    view_->UpdateAspectRation(GetEngine()->GetWindow()->GetAspectRation());
  }
  void OnEventStatusChanged(bool events_blocked) override {}

 private:
  std::vector<std::unique_ptr<Pyramid>> triangles_;
  std::unique_ptr<Shader> shader_;
  unsigned int triangles_count_;
  Image image;
  std::unique_ptr<Plane> plane;
  std::unique_ptr<Texture> plain_tex;
  std::unique_ptr<Camera> camera_;
  std::unique_ptr<PerspectiveView> view_;
  std::unique_ptr<DirectionalLight> light;
  std::unique_ptr<PointLightController> point_light_con;
};

int main(int argc, char* argv[]) {
  utils::InitLogging(argc, argv);
  auto window = Window::GetInstance({{1280, 720}, "ImGui Example", false});
  window->Init();
  auto main_engine = std::make_shared<Engine>();

  auto engine_data = EngineDataUILayer::Create();
  std::unique_ptr<Engine::CustomLayerStructure> custom_structure =
      std::make_unique<Engine::CustomLayerStructure>();
  custom_structure->structure_name = std::string("ImGui"),
  custom_structure->before_run = UI::BeforeRender;
  custom_structure->after_run = UI::AfterRender;
  custom_structure->layer_array.push_back(std::move(engine_data));

  main_engine->AttachWindow(std::move(window));
  main_engine->AttachStructure(std::move(custom_structure));
  std::unique_ptr<Layer> master_layer = std::make_unique<BenchmarkLayer>(100);
  main_engine->AttachLayer(std::move(master_layer));

  main_engine->Run();
}
