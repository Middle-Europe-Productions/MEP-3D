#include <glog/logging.h>
#include <MEP-3D/identity.hpp>

namespace mep {
unsigned int Identity::global_id_provider_ = 0;
constexpr char kEmptyClass[] = "";

Identity::Identity() : id_(-1) {}

Identity::Identity(std::string_view class_name) : Identity(class_name, "") {
  name_ = std::string(class_name_) + '_' + std::to_string(GetId());
  unique_name_ = name_ + "##" + std::to_string(id_);
}

Identity::Identity(std::string_view class_name, std::string_view name)
    : class_name_(class_name), name_(name), id_(global_id_provider_++) {
  unique_name_ = name_ + "##" + std::to_string(id_);
  VLOG(3) << "Creating identity, class_name: " << class_name_
          << ", name: " << (name.empty() ? "None" : name);
}

Identity::Identity(const Identity& id)
    : class_name_(id.class_name_),
      name_(id.name_),
      unique_name_(id.unique_name_),
      id_(global_id_provider_++) {
  VLOG(3) << "Copying identity, class_name: " << class_name_;
}

Identity::Identity(Identity&& id)
    : class_name_(id.class_name_),
      name_(id.name_),
      unique_name_(id.unique_name_),
      id_(id.id_) {
  VLOG(3) << "Moving identity, class_name: " << class_name_;
}

bool Identity::IsValid() const {
  return id_ != static_cast<unsigned int>(-1);
}

unsigned int Identity::GetId() const {
  return id_;
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
  return "\"Identity\": { \"name\": \"" + std::string(name_) +
         "\", \"class_name\": \"" + std::string(class_name_) +
         "\", \"id\": \"" + std::to_string(id_) + "\"}";
}

Identity& Identity::operator=(const Identity& id) {
  if (this != &id) {
    class_name_ = id.class_name_;
    name_ = id.name_;
    id_ = id.id_;
  }
  return *this;
}

bool Identity::operator==(const Identity& x) const {
  return this->GetId() == x.GetId();
}

bool Identity::operator<(const Identity& x) const {
  return this->GetId() < x.GetId();
}

IdentityView::IdentityView(const Identity& id)
    : class_name_(id.GetClass()), name_(id.GetName()), id_(id.GetId()) {
  DCHECK(id.IsValid());
}

IdentityView::IdentityView(std::optional<std::string_view> class_name,
                           std::optional<std::string> name,
                           std::optional<unsigned int> id)
    : class_name_(class_name), name_(name), id_(id) {}

IdentityView& IdentityView::operator=(const Identity& id) {
  class_name_ = id.GetClass();
  name_ = id.GetName();
  id_ = id.GetId();
  return *this;
}

bool IdentityView::IsValid() const {
  return class_name_.has_value() && name_.has_value() && id_.has_value();
}

bool IdentityView::HasId() const {
  return id_.has_value();
}

unsigned int IdentityView::GetId() const {
  return id_.value_or(-1);
}

bool IdentityView::HasName() const {
  return name_.has_value();
}

std::string IdentityView::GetName() const {
  return name_.value_or("");
}

bool IdentityView::HasClassName() const {
  return class_name_.has_value();
}

bool IdentityView::operator==(const IdentityView& x) const {
  return id_.has_value() && x.HasId()       ? id_.value() == x.GetId()
         : name_.has_value() && x.HasName() ? name_.value() == x.GetName()
         : class_name_.has_value() && x.HasClassName()
             ? class_name_.value() == x.GetClass()
             : false;
}

std::string_view IdentityView::GetClass() const {
  return class_name_.value_or(kEmptyClass);
}

std::string IdentityView::ToString() const {
  return "\"Identity\": { \"name\": \"" +
         std::string(name_.value_or("Unkown")) + "\", \"class_name\": \"" +
         std::string(class_name_.value_or("Unkown")) + "\", \"id\": \"" +
         std::to_string(id_.value_or(0)) + "\"}";
}

unsigned int IdentityHashFunction::operator()(const Identity& id) const {
  return id.GetId();
}

unsigned int IdentityHashFunction::operator()(const IdentityView& id) const {
  DCHECK(id.HasId());
  return id.GetId();
}

}  // namespace mep
