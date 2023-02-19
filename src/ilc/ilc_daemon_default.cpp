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
ILCClient* FindById(std::map<const Identity, ILCClient*>& connections,
                    unsigned int id) {
  for (auto& ele : connections) {
    if (ele.first.GetGlobalId() == id) {
      return ele.second;
    }
  }
  return nullptr;
}
ILCClient* FindByName(std::map<const Identity, ILCClient*>& connections,
                      std::string_view id) {
  for (auto& ele : connections) {
    if (ele.first.GetName() == std::string(id)) {
      return ele.second;
    }
  }
  return nullptr;
}
}  // namespace

ILCDaemon::ILCDaemon() {
  LOG(INFO) << "ILC Daemon created";
}

ILCDaemon::~ILCDaemon() {
  LOG(INFO) << "Exiting ILC Daemon";
}

bool ILCDaemon::SendToClient(std::unique_ptr<ILCPackage> data) {
  if (!IsILCEnabled()) {
    return false;
  }
  DCHECK(data);
  ILCDaemon::Instance().packages_.push(std::move(data));
  ILCDaemon::Instance().Update();
  return true;
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
  return true;
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
    ILCClient* client = nullptr;
    if (package->layer_id_.has_value()) {
      client = FindById(connections_, package->layer_id_.value());
    } else if (package->layer_name_.has_value()) {
      client = FindByName(connections_, package->layer_name_.value());
    }
    if (!client) {
      LOG(WARNING) << "Unkown package target, dropping";
      continue;
    }
    client->OnReceive(package->id_, package->message_);
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
