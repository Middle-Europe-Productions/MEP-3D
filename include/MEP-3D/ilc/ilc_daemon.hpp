#ifndef ILC_DAEMON_HPP
#define ILC_DAEMON_HPP

#include <MEP-3D/identity.hpp>
#include <MEP-3D/ilc/ilc_client.hpp>
#include <MEP-3D/non_copyable.hpp>

#include <memory>
#include <queue>
#include <unordered_map>

namespace mep {
class Engine;
class ILCDaemon {
 public:
  // TODO Remove after the async handler is implemented
  friend class Engine;
  static bool SendToClient(std::unique_ptr<ILCPackage> data);
  static bool Connect(const IdentityView& id, ILCClient* client);
  static bool Disconnect(const IdentityView& id);
  static bool IsActive();
  static void Init();

 private:
  ILCDaemon();
  ~ILCDaemon();
  static ILCDaemon& Instance();
  void Update(std::size_t packages = -1);

  bool active_ = false;
  std::queue<std::unique_ptr<ILCPackage>> packages_;
  std::unordered_map<IdentityView, ILCClient*, IdentityHashFunction>
      connections_;
};
};  // namespace mep
#endif