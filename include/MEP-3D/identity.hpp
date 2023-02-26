#ifndef IDENTITY_HPP
#define IDENTITY_HPP

#include <string>
#include <unordered_map>

namespace mep {
class Identity {
 public:
  Identity();
  Identity(std::string_view class_name);
  Identity(std::string_view class_name, std::string_view name);
  Identity(const Identity& id);
  bool IsValid();
  unsigned int GetId() const;
  unsigned int GetGlobalId() const;
  const std::string& GetName() const;
  const std::string& GetUniqueName() const;
  std::string_view GetClass() const;
  virtual std::string ToString() const;
  Identity& operator=(const Identity& id);
  bool operator==(const Identity& x) const;
  bool operator<(const Identity& x) const;
  static const std::unordered_map<std::string_view, int>& GetIdentityMap();
  virtual ~Identity();

 private:
  static std::unordered_map<std::string_view, int> identity_;
  static unsigned int global_id_provider_;
  std::string_view class_name_;
  std::string name_;
  std::string unique_name_;
  unsigned int global_id_;
  unsigned int id_;
};
}  // namespace mep

#endif