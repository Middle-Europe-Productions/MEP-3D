#ifndef SCENE_UI_LAYER_HPP
#define SCENE_UI_LAYER_HPP

#include <MEP-3D/layer.hpp>
#include <MEP-3D/scene.hpp>

class SceneUILayer : public Layer, public SceneObserver {
 public:
  void OnAttach(Scene* scene);
  void OnDetach();
  Scene* GetScenePtr();
  static std::unique_ptr<SceneUILayer> Create();
  static std::unique_ptr<SceneUILayer> Create(const std::string& config);
  virtual ~SceneUILayer();

 protected:
  Scene* scene_ptr_;
  SceneUILayer();
};

#endif