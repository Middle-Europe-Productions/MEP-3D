#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <MEP-3D/window.hpp>

#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

#include <MEP-3D/layer.hpp>

class EngineMonitorData {
 public:
  struct LayerData {
    Identity identity;
    std::string layer_name;
    double layer_update_time_ms = 0.0;
    double layer_draw_time_ms = 0.0;
  };
  struct FrameData {
    std::vector<LayerData> layer_data;
    double frame_time;
  };
  FrameData frame_data;
  unsigned int fps;
  std::string ToDebugJSON();
};

class EngineController;
class Engine : public std::enable_shared_from_this<Engine>, public Identity {
 public:
  Engine();
  void AttachWindow(WindowPtr window);
  const WindowPtr& GetWindow() const;
  WindowPtr& GetWindow();
  void Run();
  void AttachLayer(std::unique_ptr<Layer> obs);
  std::unique_ptr<Layer> Detachlayer(const Identity& id);
  const EngineMonitorData& GetEngineMonitor() const;
  std::unique_ptr<Layer>& operator[](std::size_t layer_index);
  bool operator==(const Identity& id);

 private:
  WindowPtr window_;
  EngineMonitorData engine_monitor_;
  std::vector<std::unique_ptr<Layer>> layers_;
};

using EnginePtr = std::shared_ptr<Engine>;

#endif