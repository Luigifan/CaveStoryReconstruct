#ifndef GAME_H_
#define GAME_H_

#include <memory> //fuck boost
#include "unit.h"

struct Player;
struct AnimatedSprite;
struct Graphics;
struct Map;
struct FirstCaveBat;

struct Game
{
  Game();
  ~Game();
  
  static units::Tile kScreenWidth;
  static units::Tile kScreenHeight;
  static units::Pixel kWindowWidth;
  static units::Pixel kWindowHeight;
  void runEventLoop();
private:
  void eventLoop();
  void update(units::MS elapsed_time_ms);
  void draw(Graphics& graphics);
  std::shared_ptr<Player> player_;
  std::shared_ptr<Map> map_;
  std::unique_ptr<FirstCaveBat> bat_;
};

#endif //GAME_H_
