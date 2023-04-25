#include <MEP-3D/mep-3d.hpp>

#include <imgui.h>

#include <MEP-3D/template/ui_element.hpp>
#include <MEP-3D/template/ui_handlers_impl.hpp>

#include <MEP-3D/common.hpp>
#include <MEP-3D/version.hpp>

#include "transfer_function.hpp"
#include "volume_renderer.hpp"

using namespace mep;

int main(int argc, char* argv[]) {
  utils::Init(argc, argv);
  tf::Register();
  auto window = Window::GetInstance(
      {{1280, 720},
       std::string("Volume Rendering ") + std::string(MEP_FULL_VERSION)});
  window->Init();
  utils::InitOpenGLogging();
  auto engine = utils::CreateEngineWithSceneUI(
      std::move(window), std::make_unique<VolumeRenderer>(),
      SceneUILayer::Create(
          utils::LoadFromFile("runtime_configurations/config.json")));
  engine->Run();
}
