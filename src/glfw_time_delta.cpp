#include <GLFW/glfw3.h>
#include <MEP-3D/time_delta.hpp>


class GlfwTimeDelta : public TimeDelta {
 public:
  GlfwTimeDelta() { last_time_ = glfwGetTime(); }
  virtual double GetTimeDelta() override {
    GLfloat now = glfwGetTime();
    double delta_time = now - last_time_;
    last_time_ = now;
    return delta_time;
  }

 private:
  double last_time_;
};

std::unique_ptr<TimeDelta> TimeDelta::GetInstance() {
  return std::make_unique<GlfwTimeDelta>();
}