#ifndef ILC_CLIENT_HPP
#define ILC_CLIENT_HPP

#include <MEP-3D/identity.hpp>

#include <optional>
#include <string>
namespace mep {
struct ILCConfig {
  bool allow_ilc = true;
};

struct ILCPackage {
  IdentityView id_;
  IdentityView target_;
  std::string message_;
  ILCPackage(const IdentityView& id,
             const IdentityView& target,
             const std::string& message)
      : id_(id), target_(target), message_(message) {}
};

class ILCClient {
 public:
  ILCClient() = default;
  virtual ~ILCClient() = default;
  virtual void OnReceive(const IdentityView& id,
                         const std::string& message) = 0;
  virtual void OnConnectionClosed() = 0;
  virtual void OnConnectionOppened() = 0;
};
};  // namespace mep
#endif