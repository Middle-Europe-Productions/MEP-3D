#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <MEP-3D/window.hpp>

#include <memory>
#include <vector>

#include <MEP-3D/layer.hpp>

class EngineMonitorData {
 public:
  struct LayerData {
    Identity identity;
    std::string layer_name;
    float layer_update_time_ms = 0.0;
    float layer_draw_time_ms = 0.0;
  };
  struct Structure {
    Identity identity;
    std::string structure_name;
    std::vector<LayerData> layer_array;
    float total_update_time;
  };
  struct FrameData {
    std::vector<LayerData> layer_data;
    std::vector<Structure> structure_data;
    float frame_time;
  };
  FrameData frame_data;
  unsigned int fps;
};

using LayerCallback = std::function<void()>;

class EngineController;
class Engine : public std::enable_shared_from_this<Engine>, public Identity {
 public:
  struct CustomLayerStructure : public Identity {
    std::string structure_name;
    LayerCallback before_run;
    LayerCallback after_run;
    std::vector<std::unique_ptr<Layer>> layer_array;
  };
  Engine();
  void AttachWindow(WindowPtr window);
  const WindowPtr& GetWindow() const;
  WindowPtr& GetWindow();
  void Run();
  void AttachLayer(std::unique_ptr<Layer> obs);
  int AttachStructure(std::unique_ptr<CustomLayerStructure> str);
  void AttachLayerToStructure(std::unique_ptr<Layer> layer,
                              int custom_layer_index);
  std::unique_ptr<Layer> Detachlayer(const Identity& id);
  const EngineMonitorData& GetEngineMonitor() const;
  std::unique_ptr<Layer>& operator[](std::size_t layer_index);
  bool operator==(const Identity& id);

 private:
  bool EvaluateLayer(const std::unique_ptr<Layer>& layer) const;
  WindowPtr window_;
  EngineMonitorData engine_monitor_;
  std::vector<std::unique_ptr<Layer>> layers_;
  std::vector<std::unique_ptr<CustomLayerStructure>> custom_layers_;
};

using EnginePtr = std::shared_ptr<Engine>;

#endif