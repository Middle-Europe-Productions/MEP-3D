#ifndef IDENTITY_HPP
#define IDENTITY_HPP

#include <string>
#include <unordered_map>

class Identity {
 public:
  Identity(const char* name);
  unsigned int GetId() const;
  unsigned int GetGlobalId() const;
  virtual std::string ToString() const;
  bool operator==(const Identity& x) const;

 private:
  static std::unordered_map<const char*, int> identity_;
  static unsigned int global_id_provider_;
  const char* identity_char_;
  unsigned int global_id_;
  unsigned int id_;
};

#endif