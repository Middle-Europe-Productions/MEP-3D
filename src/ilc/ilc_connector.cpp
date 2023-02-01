#include <MEP-3D/ilc/ilc_connector.hpp>
#include <MEP-3D/ilc/ilc_daemon.hpp>

ILCConnector::ILCConnector(const Identity& id) : id_(id) {}

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
