#ifndef OBSERVABLE_VARIABLE_HPP
#define OBSERVABLE_VARIABLE_HPP

#include <MEP-3D/observer_list.hpp>
#include <unordered_map>

namespace mep {
template <typename Type, typename Identity>
class MapObserver {
 public:
  virtual void OnVariableChanged(const Type& value,
                                 const Identity& identity) = 0;
  virtual ~MapObserver() = default;
};

template <typename Type, typename Identity>
class ObservableMap : public ObserverList<MapObserver<Type, Identity>> {
 public:
  const Type& Get(const Identity& identity) const {
    assert(variable_map_.find(identity) != variable_map_.end());
    return variable_map_.at(identity);
  }
  Type& Get(const Identity& identity) {
    assert(variable_map_.find(identity) != variable_map_.end());
    return variable_map_.at(identity);
  }
  void Set(const Identity& identity, const Type& new_type_) {
    if (variable_map_.find(identity) != variable_map_.end() &&
        variable_map_[identity] == new_type_)
      return;
    variable_map_[identity] = new_type_;
    ObserverList<MapObserver<Type, Identity>>::ForAllObservers(
        [&new_type_, &identity](MapObserver<Type, Identity>* observer) {
          observer->OnVariableChanged(new_type_, identity);
        });
  }
  void Increment(const Identity& identity, const Type& value) {
    assert(variable_map_.find(identity) != variable_map_.end());
    variable_map_[identity] += value;
    ObserverList<MapObserver<Type, Identity>>::ForAllObservers(
        [this, &identity](MapObserver<Type, Identity>* observer) {
          observer->OnVariableChanged(variable_map_.at(identity), identity);
        });
  }
  void Decrement(const Identity& identity, const Type& value) {
    assert(variable_map_.find(identity) != variable_map_.end());
    variable_map_[identity] -= value;
    ObserverList<MapObserver<Type, Identity>>::ForAllObservers(
        [this, &identity](MapObserver<Type, Identity>* observer) {
          observer->OnVariableChanged(variable_map_.at(identity), identity);
        });
  }

 private:
  std::unordered_map<Identity, Type> variable_map_;
};
}  // namespace mep

#endif