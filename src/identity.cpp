#include <MEP-3D/identity.hpp>

std::unordered_map<const char*, int> Identity::identity_ =
    std::unordered_map<const char*, int>();
unsigned int Identity::global_id_provider_ = 0;

Identity::Identity(const char* name)
    : identity_char_(name), global_id_(global_id_provider_++) {
  auto temp = identity_.find(name);
  if (temp == identity_.end()) {
    identity_[name] = 0;
    id_ = 0;
  } else {
    id_ = ++identity_[name];
  }
}
unsigned int Identity::GetId() const {
  return id_;
}

unsigned int Identity::GetGlobalId() const {
  return global_id_provider_;
}

std::string Identity::ToString() const {
  return "Identity: { name: " + std::string(identity_char_) +
         ", id: " + std::to_string(id_) +
         ", global_id: " + std::to_string(global_id_) + "} ";
}

bool Identity::operator==(const Identity& x) const {
  return this->GetId() == x.GetId() && this->GetGlobalId() == x.GetId();
}