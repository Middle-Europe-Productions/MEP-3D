#ifndef OBSERVER_LIST_HPP
#define OBSERVER_LIST_HPP

#include<list>
#include<functional>

template <typename ObserverType>
class ObserverList {
public:
	using ObserverCallback = std::function<void(ObserverType*)>;
	void AddObserver(ObserverType* obs);
	void RemoveObserver(const ObserverType* obs);
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
void ObserverList<ObserverType>::RemoveObserver(const ObserverType* obs) {
	if (obs)
		observer_list_.remove(obs);
}

template <typename ObserverType>
void ObserverList<ObserverType>::ForAllObservers(ObserverCallback callback) {
	for (auto* obs : observer_list_) {
		callback(obs);
	}
}

#endif
