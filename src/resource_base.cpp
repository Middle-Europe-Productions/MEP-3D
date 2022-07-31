#include <MEP-3D/resource_base.hpp>

std::string ToString(Status status) {
  switch (status) {
    case Status::Unknown:
      return "Unknown";
    case Status::Loading:
      return "Loading";
    case Status::Avalible:
      return "Avalible";
  }
  return "";
}

ResourceBase::ResourceBase() : status_(Status::Unknown) {}

Status ResourceBase::GetStatus() const {
  return status_.load();
}
void ResourceBase::UpdateStatus(Status status) {
  status_ = status;
}
