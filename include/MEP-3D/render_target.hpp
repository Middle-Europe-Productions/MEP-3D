#ifndef RENDER_TARGET_HPP
#define RENDER_TARGET_HPP

#include <memory>

#include <MEP-3D/camera.hpp>
#include <MEP-3D/view_base.hpp>


class RenderTarget {
 public:
  RenderTarget();
  void AddCamera(Camera* camera);
  void RemoveCamera();
  Camera* GetCamera();
  const Camera* GetCamera() const;
  void AddView(ViewBase* view);
  void RemoveView();
  ViewBase* GetView();
  const ViewBase* GetView() const;
  virtual ~RenderTarget() = default;

 private:
  ViewBase* view_;
  Camera* camera_;
};

#endif