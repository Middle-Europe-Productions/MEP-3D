#include <MEP-3D/resource_base.hpp>
#include <MEP-3D/utils.hpp>

std::string ToString(Status status) {
  switch (status) {
    case Status::NotImplemented:
      return "NotImplemented";
    case Status::Unknown:
      return "Unknown";
    case Status::Waiting:
      return "Waiting";
    case Status::Loading:
      return "Loading";
    case Status::Failed:
      return "Failed";
    case Status::Uninitialized:
      return "Uninitialized";
    case Status::Avalible:
      return "Avalible";
  }
  NOTREACHED();
  return "";
}

ResourceBase::ResourceBase() : status_(Status::Unknown) {}

Status ResourceBase::GetStatus() const {
  return status_.load();
}
void ResourceBase::UpdateStatus(Status status) {
  status_ = status;
}
