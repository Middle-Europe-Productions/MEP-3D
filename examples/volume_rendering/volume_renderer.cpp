#include "volume_renderer.hpp"
#include "common.hpp"

namespace {
constexpr char kVertexPath[] = "shaders/ray_caster.vert";
constexpr char kFragmentPath[] = "shaders/ray_caster.frag";
};  // namespace

VolumeRenderer::VolumeRenderer() : Scene(vr::kVolumeRenderer) {}

void VolumeRenderer::OnAttach() {
  auto& window = GetEngine()->GetWindow();
  if (!window) {
    LOG(ERROR) << "Window does not exist";
    return;
  }
  // Camera config
  view_ = std::make_unique<PerspectiveView>(PerspectiveView::Config(
      {glm::radians(45.0f), window->GetAspectRation(), 0.1f, 10000.0f}));
  auto config = PerspectiveCameraConfig::Create();
  config.start_position = {5.0, 3.0, -5.0};
  config.start_yaw = 135;
  config.start_pitch = -17;
  auto camera = std::make_unique<PerspectiveCamera>(config);
  // Arcball camera
  auto arcball_config = ArcballCameraConfig::Create();
  arcball_config.start_position = glm::vec3(1.0f, 1.0f, -5.0f);
  auto arcball_camera = std::make_unique<ArcballCamera>(arcball_config);

  // Init window
  window->AddView(view_.get());
  window->AddCamera(camera.get());
  window->AddObserver(camera.get());
  window->AddObserver(arcball_camera.get());
  // Shader setup
  auto shader = std::make_unique<Shader>("ray_casting");
  shader->CreateFromFile(kVertexPath, kFragmentPath);
  shaders::CacheDefaultUnifroms(*shader);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  Attach(std::move(camera), std::move(shader), std::move(arcball_camera));
}

void VolumeRenderer::OnDetach() {}

void VolumeRenderer::OnUpdate(float time_delta) {
  UpdateAll(time_delta);
}

void VolumeRenderer::OnDraw(RenderTarget& render_target) {
  glEnable(GL_BLEND);
  GetShaders()[0]->Use();
  transfer_function_texture_.Use(2);
  // TODO: Add shader observer
  GetShaders()[0]->SetUniform("step_size",
                              glm::vec3(1.0f / 256, 1.0f / 256, 1.0f / 256));
  GetShaders()[0]->SetUniform("transfer_function",
                              (int)transfer_function_texture_.GetHandler());
  DrawAll(render_target);
  transfer_function_texture_.Stop();
  GetShaders()[0]->Stop();
  glDisable(GL_BLEND);
}

void VolumeRenderer::OnReceive(const IdentityView& id,
                               const nlohmann::json& message) {
  LOG(INFO) << "Message received: " << id.ToString() << ", " << message;
  if (message.contains(vr::kTextureNode) &&
      message.contains(vr::kTextureAction)) {
    if (message[vr::kTextureAction] == vr::kTextureCreate) {
      transfer_function_texture_.SetHandler(
          static_cast<unsigned int>(message[vr::kTextureNode]));
    } else {
      transfer_function_texture_.Clear();
    }
  }
}