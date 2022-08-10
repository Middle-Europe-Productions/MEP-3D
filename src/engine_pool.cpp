#include <MEP-3D/engine_pool.hpp>

Identity EnginePool::AttachEngine(EnginePtr engine) {
  Identity id = *(Identity*)engine.get();
  Get().engine_list_[id.GetId()] = std::move(engine);
  return id;
}

Identity EnginePool::AttachEngineAndExecuteTask(EnginePtr engine, Task task) {
  auto id = EnginePool::AttachEngine(std::move(engine));
  task(EnginePool::Get().engine_list_[id.GetId()]);
  return id;
}

void EnginePool::PostTaskToAll(Task task) {
  for (auto& eng : EnginePool::Get().engine_list_) {
    task(eng.second);
  }
}

void EnginePool::PostTaskToTarget(Task task, int target_id) {
  task(EnginePool::Get().engine_list_[target_id]);
}

EnginePtr& EnginePool::GetEngine(const Identity& identity) {
  return EnginePool::Get().engine_list_[identity.GetId()];
}

EnginePool& EnginePool::Get() {
  static EnginePool engine_controller_;
  return engine_controller_;
}
