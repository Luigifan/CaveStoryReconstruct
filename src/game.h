#ifndef GAME_H_
#define GAME_H_

struct SDL_Window; //forward declaration woot

struct Game
{
  Game();
  ~Game();
  void runEventLoop();
private:
  void eventLoop();
  void update();
  void draw();
  SDL_Window* sdlScreen;
};

#endif //GAME_H_
