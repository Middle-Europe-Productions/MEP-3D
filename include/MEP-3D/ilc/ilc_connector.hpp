#ifndef ILC_CONNECTOR_HPP
#define ILC_CONNECTOR_HPP

#include <MEP-3D/ilc/ilc_client.hpp>

#include <memory>

class ILCConnector {
 public:
  ILCConnector(const Identity& id);
  virtual ~ILCConnector();
  bool Send(std::unique_ptr<ILCPackage> data);
  bool Connect(ILCClient* client);
  bool Disconnect();

 private:
  const Identity id_;
};

#endif