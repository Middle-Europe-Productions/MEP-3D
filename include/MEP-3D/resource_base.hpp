#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <atomic>
#include <string>

enum class Status { Unknown = 0, Loading = 1, Uninitialized, Avalible };

std::string ToString(Status status);

class ResourceBase {
 public:
  ResourceBase();
  Status GetStatus() const;
  void UpdateStatus(Status status);

 private:
  std::atomic<Status> status_;
};

#endif