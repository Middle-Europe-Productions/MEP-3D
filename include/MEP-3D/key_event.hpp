#ifndef KEY_EVENT_HPP
#define KEY_EVENT_HPP

#include <string>

enum class Key {
  A = 0,
  B = 1,
  C = 2,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,
  Space,
  Apostrophe,
  Comma,
  Minus,
  Period,
  Slash,
  Key0,
  Key1,
  Key2,
  Key3,
  Key4,
  Key5,
  Key6,
  Key7,
  Key8,
  Key9,
  Semicolon,
  Equal,
  LeftBracket,
  Backslash,
  RightBracket,
  Accent,
  Escape,
  Enter,
  Tab,
  Backspace,
  Insert,
  Delete,
  RightArrow,
  LeftArrow,
  DownArrow,
  UpArrow,
  CapsLock,
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
  F13,
  F14,
  F15,
  F16,
  F17,
  F18,
  F19,
  F20,
  LeftShift,
  LeftControl,
  LeftAlt,
  LeftSuper,
  RightShift,
  RightControl,
  RightAlt,
  RightSuper,
  MouseLeft,
  MouseRight,
  MouseMiddle,
  Mouse1,
  Mouse2,
  Mouse3,
  Mouse4,
  Mouse5,
  UnknownKey
};

std::string KeyToString(Key Key);

enum Action { Pressed = 0, Released, UnknownAction };
struct KeyEvent {
  Key code = Key::UnknownKey;
  Action action = Action::UnknownAction;
};

struct MouseEvent {
  float x;
  float y;
};

#endif
