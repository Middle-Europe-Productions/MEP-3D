#ifndef IDENTITY_HPP
#define IDENTITY_HPP

#include <optional>
#include <string>
#include <unordered_map>

namespace mep {
class Identity {
 public:
  Identity();
  Identity(std::string_view class_name);
  Identity(std::string_view class_name, std::string_view name);
  Identity(const Identity& id);
  Identity(Identity&& id);
  bool IsValid() const;
  unsigned int GetId() const;
  const std::string& GetName() const;
  const std::string& GetUniqueName() const;
  std::string_view GetClass() const;
  virtual std::string ToString() const;
  Identity& operator=(const Identity& id);
  bool operator==(const Identity& x) const;
  bool operator<(const Identity& x) const;
  virtual ~Identity() = default;

 private:
  static unsigned int global_id_provider_;
  std::string_view class_name_;
  std::string name_;
  std::string unique_name_;
  unsigned int id_;
};

class IdentityView {
 public:
  IdentityView(const Identity& id);
  IdentityView(std::optional<std::string_view> class_name = std::nullopt,
               std::optional<std::string> name = std::nullopt,
               std::optional<unsigned int> id = std::nullopt);
  IdentityView& operator=(const Identity& id);
  bool IsValid() const;
  bool HasId() const;
  unsigned int GetId() const;
  bool HasName() const;
  std::string GetName() const;
  bool HasClassName() const;
  bool operator==(const IdentityView& x) const;
  std::string_view GetClass() const;

  virtual std::string ToString() const;

 private:
  std::optional<std::string_view> class_name_;
  std::optional<std::string> name_;
  std::optional<unsigned int> id_;
};

class IdentityHashFunction {
 public:
  unsigned int operator()(const Identity& id) const;
  unsigned int operator()(const IdentityView& id) const;
};

}  // namespace mep

#endif