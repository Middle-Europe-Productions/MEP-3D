#include <MEP-3D/common_names.hpp>
#include <MEP-3D/layer.hpp>

namespace mep {
Layer::Layer(const ILCConfig& config) : Identity(kLayer) {
  LOG(INFO) << kLayer << ", id: " << ToString();
  if (config.allow_ilc) {
    ConnectorToILC();
  }
}

Layer::Layer(const std::string& layer_name, const ILCConfig& config)
    : Identity(kLayer, layer_name.c_str()) {
  LOG(INFO) << kLayer << ", id: " << ToString();
  if (config.allow_ilc) {
    ConnectorToILC();
  }
}

std::shared_ptr<Engine> Layer::GetEngine() const {
  return engine_;
}
Layer::~Layer() {
  LOG(INFO) << __FUNCTION__ << ", " << ToString();
  if (connection_) {
    DisconnectFromILC();
  }
}

void Layer::RegisterEngine(const std::shared_ptr<Engine>& engine) {
  LOG(INFO) << __FUNCTION__ << ", Layer: " << ToString();
  engine_ = engine;
}

bool Layer::ShouldIgnoreLayer() const {
  return false;
}

void Layer::UnregisterEngine() {
  LOG(INFO) << __FUNCTION__ << ", Layer: " << ToString();
  engine_ = nullptr;
}

void Layer::OnReceive(const IdentityView& id, const nlohmann::json& message) {
  VLOG(1) << "Received data from: " << id.ToString()
          << ", message: " << message;
}

void Layer::OnConnectionClosed() {
  VLOG(1) << "Connected to ILC was closed " << ToString();
}

void Layer::OnConnectionOppened() {
  VLOG(1) << "Successfully connected to ILC " << ToString();
}

void Layer::Send(const IdentityView& target, const std::string& message) {
  connection_->Send(std::make_unique<ILCPackage>(*this, target, message));
}

void Layer::ConnectorToILC() {
  DCHECK(!connection_);
  connection_ = std::make_unique<ILCConnector>(*this);
  connection_->Connect(this);
}

void Layer::DisconnectFromILC() {
  DCHECK(connection_);
  connection_->Disconnect();
}
}  // namespace mep
