#ifndef WINDOW_HPP
#define WINDOW_HPP

#include<memory>
#include<string>

#include "observer_list.hpp"
#include "window_observer.hpp"

struct WindowConfig {
	std::size_t width;
	std::size_t height;
	std::string name;
};

class Window: public ObserverList<WindowObserver> {
public:
	virtual bool Init() = 0;
	virtual void Close() = 0;
	virtual bool IsOpen() = 0;
	virtual void FinishLoop() = 0;
	virtual std::size_t GetHight() = 0;
	virtual std::size_t GetWidth() = 0;
	virtual ~Window() = default;
	static std::unique_ptr<Window> GetInstance(WindowConfig config);
};
using WindowPtr = std::unique_ptr<Window>;

#endif