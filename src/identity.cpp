#include <MEP-3D/identity.hpp>

std::unordered_map<std::string, int> Identity::identity_ =
    std::unordered_map<std::string, int>();
unsigned int Identity::global_id_provider_ = 0;

Identity::Identity() : class_name_(), name_(), id_(-1), global_id_(-1) {}

Identity::Identity(const char* class_name) : Identity(class_name, "") {
  name_ = class_name_ + '_' + std::to_string(GetId());
}

Identity::Identity(const char* class_name, const char* name)
    : class_name_(class_name), name_(name), global_id_(global_id_provider_++) {
  auto temp = identity_.find(class_name);
  if (temp == identity_.end()) {
    identity_[class_name] = 0;
    id_ = 0;
  } else {
    id_ = ++identity_[class_name];
  }
}

Identity::Identity(const Identity& id)
    : class_name_(id.class_name_), global_id_(id.global_id_), id_(id.id_) {}

bool Identity::IsValid() {
  return class_name_.size() == 0;
}

unsigned int Identity::GetId() const {
  return id_;
}

unsigned int Identity::GetGlobalId() const {
  return global_id_provider_;
}

const std::string& Identity::GetName() const {
  return name_;
}

std::string Identity::ToString() const {
  return "\"Identity\": { name: " + std::string(class_name_) +
         ", id: " + std::to_string(id_) +
         ", global_id: " + std::to_string(global_id_) + "}";
}

Identity& Identity::operator=(const Identity& id) {
  if (this != &id) {
    class_name_ = id.class_name_;
    name_ = id.name_;
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
  identity_[class_name_]--;
}
