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
  CameraBase() : Identity(kCamera) {}
  CameraBase(const std::string& name) : Identity(kCamera, name.c_str()) {}
  virtual glm::mat4 GetViewMatrix() const = 0;
  virtual glm::vec3 GetPosition() const = 0;
  virtual glm::vec3 GetNormalizedDirection() const = 0;
};

#endif