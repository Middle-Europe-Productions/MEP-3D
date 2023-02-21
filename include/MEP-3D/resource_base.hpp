#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <atomic>
#include <string>

namespace mep {
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
  void WaitForResource();
  ~ResourceBase();

 private:
  void EvaluateTask(Status new_status);
  std::atomic<Status> status_;
  std::atomic<bool> active_task_;
};
}  // namespace mep

#endif