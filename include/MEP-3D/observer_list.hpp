#ifndef OBSERVER_LIST_HPP
#define OBSERVER_LIST_HPP

#include <glog/logging.h>
#include <functional>
#include <list>

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
  LOG(INFO) << __FUNCTION__ << ", observer_id: " << obs->GetObsserverId();
  for (auto it = observer_list_.begin(); it != observer_list_.end(); ++it) {
    if ((*it)->IsEqual(obs)) {
      if (it != observer_list_.end()) {
        LOG(INFO) << __FUNCTION__ << ", " << __LINE__ << ", "
                  << (*it)->GetObsserverId();
        observer_list_.erase(it);
      } else {
        LOG(ERROR) << "Could not find an observer!";
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

#endif
