#include <MEP-3D/executor.hpp>

Task::Task(TaskCallback task) : task_(task) {}

void Task::Execute() {
  task_();
}

TaskWithCallback::TaskWithCallback(TaskCallback task, TaskCallback callback)
    : Task(task), callback_(callback) {}

void TaskWithCallback::Execute() {
  Task::Execute();
  callback_();
}

Executor::Executor(Type type, std::size_t number_of_thread)
    : Identity(__FUNCTION__), type_(type) {
  LOG(INFO) << "Starting the executor: " << ToString()
            << ", type: " << TypeToString()
            << ", number of threads: " << number_of_thread;
  for (std::size_t i = 0; i < number_of_thread; ++i) {
    thread_pool_.emplace_back(&Executor::RunThread, this);
  }
}

std::string Executor::TypeToString() const {
  switch (type_) {
    case Type::Blocking:
      return "Blocking";
    case Type::NonBlocking:
      return "NonBlocking";
  }
  return "Unknown";
}

void Executor::PostTask(TaskPtr task) {
  {
    std::scoped_lock lock{mutex_};
    task_queue_.push(std::move(task));
  }
  wakeup_.notify_one();
}

Executor::~Executor() {
  active_.store(false, std::memory_order_release);
  wakeup_.notify_all();
  for (auto& t : thread_pool_) {
    if (type_ == Type::Blocking) {
      t.detach();
    } else {
      t.join();
    }
  }
}

void Executor::RunThread() noexcept {
  while (true) {
    std::unique_lock lock{mutex_};
    wakeup_.wait(lock, [this]() {
      auto active = this->active_.load(std::memory_order_acquire);
      return !(this->task_queue_.empty() && active);
    });
    if (task_queue_.empty())
      break;
    auto next = std::move(task_queue_.front());
    task_queue_.pop();
    lock.unlock();
    next->Execute();
  }
}