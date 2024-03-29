#ifndef VIEW_BASE_HPP
#define VIEW_BASE_HPP

#include <glm/glm.hpp>
#include <memory>

namespace mep {
class ViewBase {
 public:
  virtual const glm::mat4& GetProjection() const = 0;
  virtual void UpdateAspectRation(float aspect_ration) = 0;
};
}  // namespace mep

#endif