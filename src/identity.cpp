#include<MEP-3D/identity.hpp>

std::unordered_map<const char*, int> Identity::identity_ = std::unordered_map<const char*, int>();

Identity::Identity(const char *name): identity_char_(name) {
    auto temp = identity_.find(name);
    if (temp == identity_.end()) {
        identity_[name] = 0;
        id_ = 0;
    }
    else {
        id_ = ++identity_[name];
    }
}
unsigned int Identity::GetId() const {
    return id_;
}

std::string Identity::ToString() const {
    return  "Identity: { name: " + std::string(identity_char_) + ", id: " + std::to_string(id_);
}