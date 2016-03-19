#include "input.h"

void Input::beginNewFrame()
{
  pressed_keys.clear();
  released_keys.clear();
}

void Input::keyUpEvent(const SDL_Event& event)
{
  released_keys[event.key.keysym.sym] = true;
  held_keys[event.key.keysym.sym] = false;
}
void Input::keyDownEvent(const SDL_Event& event)
{
  //with maps, if bools don't exist they default to false.
  //if value is bool that is
  pressed_keys[event.key.keysym.sym] = true;
  held_keys[event.key.keysym.sym] = true;
}

bool Input::wasKeyPressed(SDL_Keycode key)
{
  return pressed_keys[key];
}
bool Input::wasKeyReleased(SDL_Keycode key)
{
  return released_keys[key];
}
bool Input::isKeyHeld(SDL_Keycode key)
{
  return held_keys[key];
}
