#ifndef GAME_H_
#define GAME_H_

#include <memory> //fuck boost

struct Sprite;
struct Graphics;
struct Game
{
  Game();
  ~Game();
  void runEventLoop();
private:
  void eventLoop();
  void update();
  void draw(Graphics& graphics);
  std::unique_ptr<Sprite> sprite_;
};

#endif //GAME_H_
