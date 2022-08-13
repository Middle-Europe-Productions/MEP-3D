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
  Layer(const std::string& layer_name);
  virtual void OnAttach() = 0;
  virtual void OnDetach() = 0;
  virtual void OnUpdate(float time_delta) = 0;
  virtual void OnDraw(RenderTarget& render_target) = 0;
  const std::string& GetName() const;
  std::shared_ptr<Engine> GetEngine() const;
  virtual ~Layer();

 private:
  void RegisterEngine(const std::shared_ptr<Engine>& engine);
  std::shared_ptr<Engine> engine_;
  const std::string layer_name_;
};

#endif