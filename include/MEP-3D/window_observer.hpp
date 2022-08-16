#ifndef WINDOW_OBSERVER_HPP
#define WINDOW_OBSERVER_HPP

#include <MEP-3D/key_event.hpp>

class WindowObserver {
 public:
  virtual void OnKeyEvent(KeyEvent event) = 0;
  virtual void OnMouseEvent(MouseEvent event) = 0;
  virtual void OnEventStatusChanged(bool events_blocked) = 0;
  virtual ~WindowObserver() = default;
};

#endif