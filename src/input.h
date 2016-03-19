#ifndef INPUT_H_
#define INPUT_H_

#include <map>
#include <SDL2/SDL.h>

struct Input
{
  void beginNewFrame();

  //event handlers
  void keyUpEvent(const SDL_Event& event);
  void keyDownEvent(const SDL_Event& event);

  bool wasKeyPressed(SDL_Keycode key);
  bool wasKeyReleased(SDL_Keycode key);
  bool isKeyHeld(SDL_Keycode key);
private:
  std::map<SDL_Keycode, bool> held_keys;
  std::map<SDL_Keycode, bool> pressed_keys;
  std::map<SDL_Keycode, bool> released_keys;
};

#endif //INPUT_H_
