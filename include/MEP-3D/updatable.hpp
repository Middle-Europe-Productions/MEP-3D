#ifndef UPDATABLE_HPP
#define UPDATABLE_HPP

#include <functional>

class Updatable {
 public:
  using UpdateCallback = std::function<void()>;
  Updatable() : changed_(true) {}
  virtual void Update(float time_delta) = 0;
  virtual void Updated();
  virtual void Changed();
  virtual bool ShouldUpdate() const;
  virtual ~Updatable() = default;

 protected:
  virtual void UpdateInt(UpdateCallback update);

 private:
  bool changed_;
};

#endif