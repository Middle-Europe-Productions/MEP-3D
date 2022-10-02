#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <atomic>
#include <string>

enum class Status {
  // When the resource handler is not implemented
  NotImplemented = -1,
  // Not passed to the thread pool
  Unknown = 0,
  // Waiting for the avalible thread
  Waiting = 1,
  Loading = 2,
  // Loading failed (task exit false)
  Failed,
  // Loading success, factory passed to engine
  Uninitialized,
  // Initialized in engine
  Avalible
};

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