#ifndef OBSERVER_LIST_HPP
#define OBSERVER_LIST_HPP

#include <list>
#include <functional>

template <typename ObserverType>
class ObserverList {
public:
	using ObserverCallback = std::function<void(ObserverType*)>;
	void AddObserver(ObserverType* obs) {
		if (obs)
			observer_list_.push_back(obs);
	}
	void RemoveObserver(const ObserverType* obs) {
		if (obs)
			observer_list_.remove(obs);
	}
	void ForAll(ObserverCallback callback) {
		for (auto* obs : observer_list_) {
			callback(obs);
		}
	}
private:
	std::list<ObserverType*> observer_list_;
};

#endif
