#include <glog/logging.h>
#include <MEP-3D/identity.hpp>

namespace mep {
std::unordered_map<std::string_view, int> Identity::identity_ =
    std::unordered_map<std::string_view, int>();
unsigned int Identity::global_id_provider_ = 0;

Identity::Identity() : class_name_(), name_(), global_id_(-1), id_(-1) {}

Identity::Identity(std::string_view class_name) : Identity(class_name, "") {
  name_ = std::string(class_name_) + '_' + std::to_string(GetId());
  unique_name_ = name_ + "##" + std::to_string(global_id_);
}

Identity::Identity(std::string_view class_name, std::string_view name)
    : class_name_(class_name), name_(name), global_id_(global_id_provider_++) {
  auto temp = identity_.find(class_name);
  if (temp == identity_.end()) {
    identity_[class_name] = 0;
    id_ = 0;
  } else {
    id_ = ++identity_[class_name];
  }
  unique_name_ = name_ + "##" + std::to_string(global_id_);
  VLOG(3) << "Creating identity, class_name: " << class_name_
          << ", name: " << name_;
}

Identity::Identity(const Identity& id)
    : class_name_(id.class_name_),
      name_(id.name_),
      global_id_(global_id_provider_),
      id_(id.id_) {}

bool Identity::IsValid() {
  return class_name_.size() == 0;
}

unsigned int Identity::GetId() const {
  return id_;
}

unsigned int Identity::GetGlobalId() const {
  return global_id_;
}

const std::string& Identity::GetName() const {
  return name_;
}

const std::string& Identity::GetUniqueName() const {
  return unique_name_;
}

std::string_view Identity::GetClass() const {
  return class_name_;
}

std::string Identity::ToString() const {
  return "\"Identity\": { \"name\": " + std::string(name_) +
         ", \"class_name\": " + std::string(class_name_) +
         ", \"id\": " + std::to_string(id_) +
         ", \"global_id\": " + std::to_string(global_id_) + "}";
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
  return this->GetId() == x.GetId() && this->GetGlobalId() == x.GetGlobalId();
}

bool Identity::operator<(const Identity& x) const {
  return this->GetGlobalId() < x.GetGlobalId();
}

const std::unordered_map<std::string_view, int>& Identity::GetIdentityMap() {
  return identity_;
}

Identity::~Identity() {
  identity_[class_name_]--;
}
}  // namespace mep
