#ifndef UPDATABLE_HPP
#define UPDATABLE_HPP

#include <functional>

class Updatable {
public:
	using UpdateCallback = std::function<void()>;
	Updatable() : changed_(true) {}
	virtual void Update() = 0;
	virtual void Updated() {
		changed_ = false;
	}
	virtual void Changed() {
		changed_ = true;
	}
	virtual bool ShouldUpdate() const {
		return changed_;
	}
	virtual ~Updatable() = default;
protected:
	virtual void UpdateInt(UpdateCallback update) {
		if (ShouldUpdate()) {
			update();
			Updated();
		}
	}
private:
	bool changed_;
};

#endif