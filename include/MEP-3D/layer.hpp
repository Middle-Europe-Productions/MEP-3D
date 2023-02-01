#ifndef LAYER_HPP
#define LAYER_HPP

#include <MEP-3D/identity.hpp>
#include <MEP-3D/ilc/ilc_connector.hpp>
#include <MEP-3D/render_target.hpp>
#include <MEP-3D/time_delta.hpp>

class Engine;
class Layer : public Identity, public ILCClient {
 public:
  friend class Engine;
  Layer(const ILCConfig& config = {true});
  Layer(const std::string& layer_name, const ILCConfig& config = {true});
  virtual void OnAttach() = 0;
  virtual void OnDetach() = 0;
  virtual void OnUpdate(float time_delta) = 0;
  virtual void OnDraw(RenderTarget& render_target) = 0;
  virtual void OnReceive(const Identity& id, const std::string& message);
  virtual void OnConnectionClosed();
  virtual void OnConnectionOppened();
  void Send(int id, const std::string& message);
  void Send(std::string_view layer_name, const std::string& message);
  std::shared_ptr<Engine> GetEngine() const;
  virtual ~Layer();

 private:
  // ILC
  void ConnectorToILC();
  void DisconnectFromILC();

  void RegisterEngine(const std::shared_ptr<Engine>& engine);
  void UnregisterEngine();
  virtual bool ShouldIgnoreLayer() const;
  std::shared_ptr<Engine> engine_;
  std::unique_ptr<ILCConnector> connection_;
};

#endif