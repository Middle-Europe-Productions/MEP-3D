#include <MEP-3D/resource_base.hpp>
#include <MEP-3D/utils.hpp>

#include <glog/logging.h>

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

ResourceBase::ResourceBase() : status_(Status::Unknown), active_task_(false) {}

Status ResourceBase::GetStatus() const {
  return status_.load(std::memory_order_acquire);
}
void ResourceBase::UpdateStatus(Status status) {
  status_.store(status, std::memory_order_release);
  EvaluateTask(status);
}

void ResourceBase::WaitForResource() {
  auto status = GetStatus();
  if (status == Status::Loading || status == Status::Waiting) {
    LOG(WARNING) << "Shutting down active resource. Waiting";
  }
  active_task_.wait(true);
}

ResourceBase::~ResourceBase() {
  WaitForResource();
}

void ResourceBase::EvaluateTask(Status new_status) {
  switch (new_status) {
    case Status::Waiting:
    case Status::Loading:
      active_task_.store(true, std::memory_order_release);
      active_task_.notify_all();
    case Status::Failed:
    case Status::Uninitialized:
    case Status::Avalible:
      active_task_.store(false, std::memory_order_release);
      active_task_.notify_all();
  }
}
