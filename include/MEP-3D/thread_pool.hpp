#ifndef THREAD_TOOL_HPP
#define THREAD_TOOL_HPP

#include <MEP-3D/executor.hpp>
#include <MEP-3D/identity.hpp>
#include <MEP-3D/non_copyable.hpp>
#include <MEP-3D/utils.hpp>

#include <glog/logging.h>

#include <unordered_map>

namespace mep {
class ThreadPool : public NonCopyable {
 public:
  static ThreadPool& Get();
  static void PostTask(Executors location, TaskPtr task);
  static void PostTaskWithCallback(Executors location,
                                   TaskWithCallbackPtr task);

 private:
  ThreadPool();
  std::unordered_map<Executors, ExecutorPtr> executor_pool_;
};
}  // namespace mep

#endif