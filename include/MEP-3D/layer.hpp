#ifndef LAYER_HPP
#define LAYER_HPP

#include <MEP-3D/identity.hpp>
#include <MEP-3D/render_target.hpp>
#include <MEP-3D/time_delta.hpp>

class Engine;
class Layer : public Identity {
 public:
  friend class Engine;
  Layer();
  virtual void OnAttach() = 0;
  virtual void OnDetach() = 0;
  virtual void OnUpdate(unsigned int time_delta) = 0;
  virtual void OnDraw(RenderTarget& render_target) = 0;
  std::shared_ptr<Engine> GetEngine() const;
  virtual ~Layer();

 private:
  void RegisterEngine(const std::shared_ptr<Engine>& engine);
  std::shared_ptr<Engine> engine_;
};

#endif