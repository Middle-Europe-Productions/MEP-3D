#ifndef ENGINE_POOL_HPP
#define ENGINE_POOL_HPP

#include <MEP-3D/engine.hpp>

using Task = std::function<void(EnginePtr&)>;

class EnginePool {
 public:
  static Identity AttachEngine(EnginePtr engine);
  static Identity AttachEngineAndExecuteTask(EnginePtr engine, Task task);
  static void PostTaskToAll(Task task);
  static void PostTaskToTarget(Task task, int target_id);
  static EnginePtr& GetEngine(const Identity& identity);

 private:
  static EnginePool& Get();
  std::unordered_map<unsigned int, EnginePtr> engine_list_;
};

#define ENGINE_RUN [](EnginePtr& engine) { engine->Run(); }

#endif