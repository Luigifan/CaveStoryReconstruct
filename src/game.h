#ifndef GAME_H_
#define GAME_H_

#include <memory> //fuck boost

struct Player;
struct AnimatedSprite;
struct Graphics;
struct Map;

struct Game
{
  Game();
  ~Game();
  static int kTileSize;
  void runEventLoop();
private:
  void eventLoop();
  void update(int elapsed_time_ms);
  void draw(Graphics& graphics);
  std::shared_ptr<Player> player_;
  std::shared_ptr<Map> map_;
};

#endif //GAME_H_
