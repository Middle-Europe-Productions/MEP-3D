#ifndef WINDOW_HPP
#define WINDOW_HPP

#include<memory>
#include<string>

#include<MEP-3D/observer_list.hpp>
#include<MEP-3D/window_observer.hpp>
#include<MEP-3D/render_target.hpp>
#include<MEP-3D/color.hpp>

struct WindowConfig {
	std::size_t width;
	std::size_t height;
	std::string name;
};

class Window: public ObserverList<WindowObserver>, public RenderTarget {
public:
	virtual bool Init() = 0;
	virtual void Close() = 0;
	virtual bool IsOpen() = 0;
	virtual void Clear(Color color = Black) = 0;
	virtual void FinishLoop() = 0;
	virtual std::size_t GetHight() = 0;
	virtual std::size_t GetWidth() = 0;
	virtual std::size_t GetBufferHight() = 0;
	virtual std::size_t GetBufferWidth() = 0;
	virtual ~Window() = default;
	static std::unique_ptr<Window> GetInstance(WindowConfig config);
};
using WindowPtr = std::unique_ptr<Window>;

#endif