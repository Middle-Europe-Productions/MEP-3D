#include <MEP-3D/features.hpp>
#include <MEP-3D/ilc/ilc_daemon.hpp>

#include <glog/logging.h>

namespace {
bool IsILCEnabled() {
  return feature::IsFeatureEnabled(feature::kUseILCConnectivity);
}
std::string ILCPackageToString(const ILCPackage& data) {
  std::string output = "\"ILCPackage\" : {";
  output +=
      " \"layer_id\" : " + std::to_string(data.layer_id_.value_or(-1)) + ",\n";
  output += " \"layer_name\" : \"" +
            std::string(data.layer_name_.value_or("null")) + "\",\n";
  output += " \"id\" : " + data.id_.ToString() + ",\n";
  output += " \"message\" : \"" + data.message_ + "\"\n}";
  return output;
}
}  // namespace

ILCDaemon::ILCDaemon() {
  LOG(INFO) << "ILC Daemon created";
}

ILCDaemon::~ILCDaemon() {
  LOG(INFO) << "Exiting ILC Daemon";
  for (auto& connection : connections_) {
    connection.second->OnConnectionClosed();
  }
}

bool ILCDaemon::SendToClient(std::unique_ptr<ILCPackage> data) {
  if (!IsILCEnabled()) {
    return false;
  }
  DCHECK(data);
  ILCDaemon::Instance().packages_.push(std::move(data));
  ILCDaemon::Instance().Update();
}

bool ILCDaemon::Connect(const Identity& id, ILCClient* client) {
  if (!IsILCEnabled()) {
    return false;
  }
  DCHECK(client);
  auto con_eval = Instance().connections_.find(id);
  if (con_eval != Instance().connections_.end()) {
    VLOG(3) << "Connection with " << id.ToString() << " already exists!";
    return false;
  }
  Instance().connections_[id] = client;
  client->OnConnectionOppened();
}

bool ILCDaemon::Disconnect(const Identity& id) {
  if (!IsILCEnabled()) {
    return false;
  }
  auto con_eval = Instance().connections_.find(id);
  if (con_eval != Instance().connections_.end()) {
    VLOG(3) << "Could not find " << id.ToString() << " in active connections!";
    return false;
  }
  con_eval->second->OnConnectionClosed();
  Instance().connections_.erase(con_eval);
  return true;
}

bool ILCDaemon::IsActive() {
  if (!IsILCEnabled()) {
    return false;
  }
  return Instance().active_;
}

void ILCDaemon::Update(std::size_t packages_) {
  while (!Instance().packages_.empty() && packages_ > 0) {
    auto package = std::move(Instance().packages_.back());
    Instance().packages_.pop();
    VLOG(9) << ILCPackageToString(*package);
  }
}

void ILCDaemon::Init() {
  if (IsILCEnabled()) {
    Instance().active_ = true;
    LOG(INFO) << "ILC Daemon initialized";
  }
}

ILCDaemon& ILCDaemon::Instance() {
  static ILCDaemon instance;
  return instance;
}
