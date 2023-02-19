#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <MEP-3D/identity.hpp>
#include <MEP-3D/non_copyable.hpp>

#include <glog/logging.h>

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

enum class Executors : unsigned int { Resource = 0, Count };

using TaskCallback = std::function<bool()>;
using TaskNotifyCallback = std::function<void(bool)>;

class Task {
 public:
  Task(TaskCallback task);
  virtual void Execute();
  virtual ~Task() = default;

 protected:
  TaskCallback task_;
};
using TaskPtr = std::unique_ptr<Task>;

class TaskWithCallback : public Task {
 public:
  TaskWithCallback(TaskCallback task, TaskNotifyCallback on_complete);
  void Execute() override;

 private:
  TaskNotifyCallback callback_;
};
using TaskWithCallbackPtr = std::unique_ptr<TaskWithCallback>;

class Executor : public NonCopyable, public Identity {
 public:
  enum class Type { Blocking = 0, NonBlocking };
  Executor(Type type, std::size_t number_of_thread);
  std::string TypeToString() const;
  void PostTask(TaskPtr task);
  ~Executor();

 private:
  void RunThread() noexcept;
  const Executor::Type type_;
  std::vector<std::thread> thread_pool_;
  std::condition_variable wakeup_;
  std::atomic<bool> active_;
  std::mutex mutex_;
  std::queue<TaskPtr> task_queue_;
};
using ExecutorPtr = std::unique_ptr<Executor>;

#endif