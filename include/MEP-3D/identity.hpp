#ifndef IDENTITY_HPP
#define IDENTITY_HPP

#include <string>
#include <unordered_map>

class Identity {
 public:
  Identity();
  Identity(const char* name);
  Identity(const Identity& id);
  bool IsValid();
  unsigned int GetId() const;
  unsigned int GetGlobalId() const;
  virtual std::string ToString() const;
  Identity& operator=(const Identity& id);
  bool operator==(const Identity& x) const;
  static const std::unordered_map<std::string, int>& GetIdentityMap();
  virtual ~Identity();

 private:
  static std::unordered_map<std::string, int> identity_;
  static unsigned int global_id_provider_;
  std::string identity_char_;
  unsigned int global_id_;
  unsigned int id_;
};

#endif