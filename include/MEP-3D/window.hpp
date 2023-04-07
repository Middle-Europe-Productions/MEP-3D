#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <MEP-3D/color.hpp>
#include <MEP-3D/observer_list.hpp>
#include <MEP-3D/render_target.hpp>
#include <MEP-3D/vector.hpp>
#include <MEP-3D/window_observer.hpp>
#include <memory>
#include <string>

namespace mep {
struct WindowConfig {
  Vec2u size;
  std::string name;
  bool use_vsync = true;
};

class Window : public ObserverList<WindowObserver>, public RenderTarget {
 public:
  virtual bool Init() = 0;
  virtual void Close() = 0;
  virtual bool IsOpen() = 0;
  virtual void BlockEvents(bool status = false,
                           Key exception = Key::UnknownKey) = 0;
  virtual void Clear(Color color = Black) = 0;
  virtual void StartLoop() = 0;
  virtual void FinishLoop() = 0;
  virtual Vec2i GetSize() = 0;
  virtual Vec2i GetBufferSize() = 0;
  virtual float GetAspectRation() = 0;
  virtual void* GetNativeWindow() = 0;
  virtual void UpdateWindowName(const std::string& name) = 0;
  virtual void UpdateVSync(bool enabled) = 0;
  virtual const WindowConfig& GetConfig() const = 0;
  virtual ~Window() = default;
  static std::unique_ptr<Window> GetInstance(WindowConfig config);
};
using WindowPtr = std::unique_ptr<Window>;
}  // namespace mep

#endif