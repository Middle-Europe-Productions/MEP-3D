#ifndef UTILS_ENGINE_HPP
#define UTILS_ENGINE_HPP

#include <MEP-3D/engine.hpp>
#include <MEP-3D/scene.hpp>
#include <MEP-3D/template/scene_ui_layer.hpp>

namespace mep {
namespace utils {
std::shared_ptr<Engine> CreateEngine(std::unique_ptr<Window> window,
                                     std::unique_ptr<Scene> scene);
std::shared_ptr<Engine> CreateEngineWithSceneUI(
    std::unique_ptr<Window> window,
    std::unique_ptr<Scene> scene,
    std::unique_ptr<SceneUILayer> ui_layer);
}  // namespace utils
}  // namespace mep

#endif