#ifndef CAMERA_BASE_HPP
#define CAMERA_BASE_HPP

#include <glm/glm.hpp>

#include <MEP-3D/common_names.hpp>
#include <MEP-3D/identity.hpp>
#include <MEP-3D/observable_map.hpp>
#include <MEP-3D/updatable.hpp>
#include <MEP-3D/window_observer.hpp>

enum class CameraVariables { Position, Direction };

std::string ToString(const CameraVariables& camera_variable);

using CameraObserver = MapObserver<glm::vec3, CameraVariables>;

class CameraBase : public Identity,
                   public Updatable,
                   public WindowObserver,
                   public ObservableMap<glm::vec3, CameraVariables> {
 public:
  CameraBase(std::string_view identity);
  CameraBase(std::string_view identity, std::string_view name);
  void DisableEvents();
  void EnableEvents();
  bool IsDisabled() const;
  virtual glm::mat4 GetViewMatrix() const = 0;
  virtual glm::vec3 GetPosition() const;
  virtual glm::vec3 GetNormalizedDirection() const;

 private:
  bool disabled_ = true;
};

#endif