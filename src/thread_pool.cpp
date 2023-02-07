#include <MEP-3D/thread_pool.hpp>

ThreadPool& ThreadPool::Get() {
  static ThreadPool thread_pool;
  return thread_pool;
}
void ThreadPool::PostTask(Executors location, TaskPtr task) {
  Get().executor_pool_[location]->PostTask(std::move(task));
}
void ThreadPool::PostTaskWithCallback(Executors location,
                                      TaskWithCallbackPtr task) {
  Get().executor_pool_[location]->PostTask(std::move(task));
}

ThreadPool::ThreadPool() {
  LOG(INFO) << "Thread pool initialized!";
  for (Executors exe = Executors::Resource; exe != Executors::Count;
       exe = utils::IncEnum(exe)) {
    // TOOD: Add config
    executor_pool_[exe] = std::make_unique<Executor>(
        Executor::Type::NonBlocking, std::thread::hardware_concurrency());
  }
}
