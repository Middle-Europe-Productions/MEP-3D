#ifndef RENDER_TARGET_HPP
#define RENDER_TARGET_HPP

#include <memory>

#include <MEP-3D/perspective_camera.hpp>
#include <MEP-3D/view_base.hpp>

namespace mep {
class RenderTarget {
 public:
  RenderTarget();
  void AddCamera(CameraBase* camera);
  void RemoveCamera();
  CameraBase* GetCamera();
  const CameraBase* GetCamera() const;
  void AddView(ViewBase* view);
  void RemoveView();
  ViewBase* GetView();
  const ViewBase* GetView() const;
  virtual ~RenderTarget() = default;

 private:
  ViewBase* view_;
  CameraBase* camera_;
};
}  // namespace mep

#endif