#ifndef SPACE_ACTION_HPP
#define SPACE_ACTION_HPP

#include <iostream>
#include <memory>
#include <queue>
#include <unordered_map>

#include <glm/glm.hpp>

#include <MEP-3D/common.hpp>
#include <MEP-3D/vector.hpp>

namespace mep {
class ModelController {
 public:
  ModelController();
  void Transform(float x, float y, float z);
  void Transform(const Vec3f& vec);
  void Rotate(const Vec3f& vec);
  void Rotate(float degrees, Axis axis);
  void Scale(const Vec3f& val);
  void Scale(float x, float y, float z);
  void ResetModel();
  glm::mat4& GetModel();
  const glm::mat4& GetModel() const;
  const glm::vec3& GetRotationTracker() const;
  std::string ToString() const;

 private:
  glm::mat4 model_;
  glm::vec3 rotation_tracker_deg_;
};
}  // namespace mep

#endif