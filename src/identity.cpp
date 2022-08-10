#include <MEP-3D/identity.hpp>

std::unordered_map<std::string, int> Identity::identity_ =
    std::unordered_map<std::string, int>();
unsigned int Identity::global_id_provider_ = 0;

Identity::Identity() : identity_char_(), id_(-1), global_id_(-1) {}

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

Identity::Identity(const Identity& id)
    : identity_char_(id.identity_char_),
      global_id_(id.global_id_),
      id_(id.id_) {}

bool Identity::IsValid() {
  return identity_char_.size() == 0;
}

unsigned int Identity::GetId() const {
  return id_;
}

unsigned int Identity::GetGlobalId() const {
  return global_id_provider_;
}

std::string Identity::ToString() const {
  return "\"Identity\": { name: " + std::string(identity_char_) +
         ", id: " + std::to_string(id_) +
         ", global_id: " + std::to_string(global_id_) + "}";
}

Identity& Identity::operator=(const Identity& id) {
  if (this != &id) {
    identity_char_ = id.identity_char_;
    global_id_ = id.global_id_;
    id_ = id.id_;
  }
  return *this;
}

bool Identity::operator==(const Identity& x) const {
  return this->GetId() == x.GetId() && this->GetGlobalId() == x.GetId();
}

const std::unordered_map<std::string, int>& Identity::GetIdentityMap() {
  return identity_;
}

Identity::~Identity() {
  identity_[identity_char_]--;
}
