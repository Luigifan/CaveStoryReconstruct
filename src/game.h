#ifndef GAME_H_
#define GAME_H_

struct Game
{
  Game();
  ~Game();
  void runEventLoop();
private:
  void eventLoop();
  void update();
  void draw();
};

#endif //GAME_H_
