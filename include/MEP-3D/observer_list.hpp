#ifndef OBSERVER_LIST_HPP
#define OBSERVER_LIST_HPP

#include <glog/logging.h>
#include <functional>
#include <list>

namespace mep {
template <typename ObserverType>
class ObserverList {
 public:
  using ObserverCallback = std::function<void(ObserverType*)>;
  void AddObserver(ObserverType* obs);
  void RemoveObserver(ObserverType* obs);
  void ForAllObservers(ObserverCallback callback);

 private:
  std::list<ObserverType*> observer_list_;
};

template <typename ObserverType>
void ObserverList<ObserverType>::AddObserver(ObserverType* obs) {
  if (obs)
    observer_list_.push_back(obs);
}

template <typename ObserverType>
void ObserverList<ObserverType>::RemoveObserver(ObserverType* obs) {
  if (!obs)
    return;
  VLOG(3) << __FUNCTION__ << ", observer_id: " << obs->GetObserverId();
  for (auto it = observer_list_.begin(); it != observer_list_.end(); ++it) {
    if ((*it)->IsEqual(obs)) {
      if (it != observer_list_.end()) {
        VLOG(3) << __FUNCTION__ << ", " << __LINE__ << ", "
                << (*it)->GetObserverId();
        observer_list_.erase(it);
      }
      return;
    }
  }
}

template <typename ObserverType>
void ObserverList<ObserverType>::ForAllObservers(ObserverCallback callback) {
  for (auto* obs : observer_list_) {
    callback(obs);
  }
}
}  // namespace mep

#endif
