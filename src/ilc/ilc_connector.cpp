#include <MEP-3D/ilc/ilc_connector.hpp>
#include <MEP-3D/ilc/ilc_daemon.hpp>

namespace mep {
ILCConnector::ILCConnector(const IdentityView& id) : id_(id) {}

ILCConnector::~ILCConnector() {}

bool ILCConnector::Send(std::unique_ptr<ILCPackage> data) {
  return ILCDaemon::SendToClient(std::move(data));
}

bool ILCConnector::Connect(ILCClient* client) {
  return ILCDaemon::Connect(id_, client);
}

bool ILCConnector::Disconnect() {
  return ILCDaemon::Disconnect(id_);
}
}  // namespace mep
