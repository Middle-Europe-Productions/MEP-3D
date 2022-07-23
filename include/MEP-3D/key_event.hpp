#ifndef KEY_EVENT_HPP
#define KEY_EVENT_HPP

enum Keyboard {
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
  UnknownKey
};

enum Action { Pressed = 0, Released, UnknownAction };
struct KeyEvent {
  Keyboard code = Keyboard::UnknownKey;
  Action action = Action::UnknownAction;
};

struct MouseEvent {
  double x;
  double y;
};

#endif
