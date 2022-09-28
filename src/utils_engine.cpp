#include <MEP-3D/common_names.hpp>
#include <MEP-3D/engine.hpp>
#include <MEP-3D/scene.hpp>
#include <MEP-3D/template/engine_data_ui_layer.hpp>
#include <MEP-3D/user_interface.hpp>
#include <MEP-3D/utils_engine.hpp>

namespace utils {
void InitMepLogging(const char* program_name) {
  FLAGS_logtostderr = 1;
  google::InitGoogleLogging(program_name);
}
std::shared_ptr<Engine> CreateEngineWithSceneUI(
    std::unique_ptr<Scene> scene,
    std::unique_ptr<SceneUILayer> ui_layer) {
  DCHECK(scene);
  DCHECK(ui_layer);
  auto main_engine = std::make_shared<Engine>();

  auto engine_data = EngineDataUILayer::Create();
  std::unique_ptr<Engine::CustomLayerStructure> custom_structure =
      std::make_unique<Engine::CustomLayerStructure>();
  custom_structure->structure_name = std::string(kDefaultImGuiName),
  custom_structure->before_run = UI::BeforeRender;
  custom_structure->after_run = UI::AfterRender;
  custom_structure->layer_array.push_back(std::move(engine_data));

  main_engine->AttachStructure(std::move(custom_structure));

  scene->AddObserver(ui_layer.get());
  main_engine->AttachLayer(std::move(scene));
  main_engine->AttachLayerToStructure(std::move(ui_layer), 0, true);

  return std::move(main_engine);
}

};  // namespace utils