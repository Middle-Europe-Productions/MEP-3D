#include <GL/glew.h>
#include <MEP-3D/utils.hpp>

#include <glog/logging.h>

glm::vec3 utils::AxisToGlm(Axis axis) {
  switch (axis) {
    case Axis::X:
      return glm::vec3(1.0f, 0.0f, 0.0f);
    case Axis::Y:
      return glm::vec3(0.0f, 1.0f, 0.0f);
    case Axis::Z:
      return glm::vec3(0.0f, 0.0f, 1.0f);
  }
}

float utils::DegToRadians(float degress) {
  return degress * 3.14159265f / 180.0f;
}
