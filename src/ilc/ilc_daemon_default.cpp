#include <MEP-3D/features.hpp>
#include <MEP-3D/ilc/ilc_daemon.hpp>

#include <glog/logging.h>

namespace mep {
namespace {
bool IsILCEnabled() {
  return feature::IsFeatureEnabled(feature::kUseILCConnectivity);
}
std::string ILCPackageToString(const ILCPackage& data) {
  std::string output = "\"ILCPackage\" : {";
  output += " \"source\" : " + data.id_.ToString() + ",\n";
  output += " \"target\" : " + data.target_.ToString() + ",\n";
  output += " \"message\" : \"" + data.message_.dump() + "\"\n}";
  return output;
}
}  // namespace

ILCDaemon::ILCDaemon() {
  LOG(INFO) << "[ILC] Daemon created";
}

ILCDaemon::~ILCDaemon() {
  LOG(INFO) << "[ILC] Exiting Daemon";
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

bool ILCDaemon::Connect(const IdentityView& id, ILCClient* client) {
  if (!IsILCEnabled()) {
    return false;
  }
  DCHECK(client && id.IsValid());
  auto con_eval = Instance().connections_.find(id);
  if (con_eval != Instance().connections_.end()) {
    VLOG(3) << "[ILC] Connection with " << id.ToString() << " already exists!";
    return false;
  }
  VLOG(3) << "[ILC] New connection " << id.ToString();
  Instance().connections_[id] = client;
  client->OnConnectionOppened();
  return true;
}

bool ILCDaemon::Disconnect(const IdentityView& id) {
  if (!IsILCEnabled()) {
    return false;
  }
  auto con_eval = Instance().connections_.find(id);
  if (con_eval == Instance().connections_.end()) {
    VLOG(3) << "[ILC] Could not find " << id.ToString()
            << " in active connections!";
    return false;
  }
  VLOG(3) << "[ILC] Connection lost " << id.ToString();
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

void ILCDaemon::Update(std::size_t packages) {
  while (!Instance().packages_.empty() && packages-- > 0) {
    auto package = std::move(Instance().packages_.back());
    Instance().packages_.pop();
    VLOG(5) << "[ILC] Sending " << ILCPackageToString(*package);
    if (!package->target_.HasId() && !package->target_.HasClassName() &&
        !package->target_.HasName()) {
      VLOG(3) << "[ILC] Detected broadcast package";
      for (auto& con : Instance().connections_) {
        con.second->OnReceive(package->id_, package->message_);
      }
      continue;
    }
    if (package->target_.HasId()) {
      auto client = connections_.find(package->target_);
      if (client == connections_.end()) {
        LOG(WARNING) << "[ILC] Unkown package target, dropping";
        continue;
      }
      client->second->OnReceive(package->id_, package->message_);
      continue;
    } else {
      for (auto& con : Instance().connections_) {
        if (package->target_.HasName() &&
            con.first.GetName() == package->target_.GetName()) {
          VLOG(5) << "Target found, target: " << con.first.GetName()
                  << ", source: " << package->target_.GetName();
          con.second->OnReceive(package->id_, package->message_);
          continue;
        }
        if (package->target_.HasClassName() &&
            con.first.GetClass() == package->target_.GetClass()) {
          VLOG(5) << "Target found, target: " << package->target_.ToString()
                  << ", source: " << con.first.ToString();
          con.second->OnReceive(package->id_, package->message_);
          continue;
        }
      }
      continue;
    }
    LOG(WARNING) << "[ILC] Could not find a package target, unhandled package";
  }
}

void ILCDaemon::Init() {
  if (IsILCEnabled()) {
    Instance().active_ = true;
    LOG(INFO) << "[ILC] Daemon initialized";
  }
}

ILCDaemon& ILCDaemon::Instance() {
  static ILCDaemon instance;
  return instance;
}
}  // namespace mep
