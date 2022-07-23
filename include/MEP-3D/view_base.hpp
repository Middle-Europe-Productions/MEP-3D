#ifndef VIEW_BASE_HPP
#define VIEW_BASE_HPP

#include <glm/glm.hpp>
#include <memory>

class ViewBase {
 public:
  virtual const glm::mat4& GetProjection() const = 0;
};

#endif