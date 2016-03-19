#ifndef PLAYER_H_
#define PLAYER_H_

#include <memory>
#include <map>
#include "animated_sprite.h"

struct Graphics;

struct Player
{
  Player(Graphics& graphics, float x, float y);
  void update(int elapsed_time_ms);
  void draw(Graphics& graphics);

  void startMovingLeft();
  void startMovingRight();
  void stopMoving();
private:
  enum MotionType{
    STANDING, WALKING
  };
  enum HorizontalFacing {
    LEFT, RIGHT
  };
  struct SpriteState {
    SpriteState(MotionType type = STANDING, HorizontalFacing facing = LEFT)
    {
      motion_type = type;
      horizontal_facing = facing;
    }
    MotionType motion_type;
    HorizontalFacing horizontal_facing;
  };
  //friend because SpriteState is a private friend class of player
  friend bool operator<(const SpriteState& a, const SpriteState& b);
  void initializeSprites(Graphics& graphics);
  SpriteState getSpriteState();

  float x_, y_;
  float acceleration_x_;
  float velocity_x_;
  HorizontalFacing horizontal_facing_;

  std::map<SpriteState, std::unique_ptr<Sprite>> sprites_;
};


#endif //PLAYER_H_
