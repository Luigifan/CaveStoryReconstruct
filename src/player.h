#ifndef PLAYER_H_
#define PLAYER_H_

#include <memory>
#include "animated_sprite.h"

struct Graphics;

struct Player
{
  Player(Graphics& graphics, int x, int y);
  void update(int elapsed_time_ms);
  void draw(Graphics& graphics);

  void startMovingLeft();
  void startMovingRight();
  void stopMoving();
private:
  int x_, y_;
  float acceleration_x_;
  float velocity_x_;
  std::unique_ptr<AnimatedSprite> sprite_; //fuck your boost shit
};


#endif //PLAYER_H_
