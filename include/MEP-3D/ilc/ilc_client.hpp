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
  std::optional<int> layer_id_;
  std::optional<std::string_view> layer_name_;
  const Identity& id_;
  const std::string& message_;
  ILCPackage(std::optional<int> layer_id,
             std::optional<std::string_view> layer_name,
             const Identity& id,
             const std::string& message)
      : layer_id_(layer_id),
        layer_name_(layer_name),
        id_(id),
        message_(message) {}
};

class ILCClient {
 public:
  ILCClient() = default;
  virtual ~ILCClient() = default;
  virtual void OnReceive(const Identity& id, const std::string& message) = 0;
  virtual void OnConnectionClosed() = 0;
  virtual void OnConnectionOppened() = 0;
};
};  // namespace mep
#endif