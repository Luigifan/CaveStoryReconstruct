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

  void lookUp();
  void lookDown();
  void lookHorizontal();

  void startJump();
  void stopJump();
private:
  enum MotionType : int{
    FIRST_MOTION_TYPE = 0,
    STANDING = 0, WALKING = 1, JUMPING = 2, FALLING = 3,
    LAST_MOTION_TYPE = 4
  };
  enum HorizontalFacing {
    FIRST_HORIZONTAL_FACING = 0,
    LEFT = 0, RIGHT = 1,
    LAST_HORIZONTAL_FACING = 2
  };
  enum VerticalFacing{
    FIRST_VERTICAL_FACING = 0,
    UP = 0, DOWN = 1, HORIZONTAL = 2,
    LAST_VERTICAL_FACING = 3
  };
  struct SpriteState {
    SpriteState(MotionType type = STANDING, HorizontalFacing facing = LEFT, VerticalFacing vfacing = HORIZONTAL)
    {
      motion_type = type;
      horizontal_facing = facing;
      vertical_facing = vfacing;
    }
    MotionType motion_type;
    HorizontalFacing horizontal_facing;
    VerticalFacing vertical_facing;
  };
  struct Jump {
    Jump()
    {
      time_remaining_ms_ = 0;
      active_ = false;
    }
    void update(int elapsed_time_ms);
    void reset();
    void reactivate()
    {
      active_ = time_remaining_ms_ > 0;
    } //can only reactivate if time remaining > 0
    void deactivate()
    {
      active_ = false;
    }
    bool active() const {return active_;}
  private:
    int time_remaining_ms_;
    bool active_;
  };

  //friend because SpriteState is a private friend class of player
  friend bool operator<(const SpriteState& a, const SpriteState& b);
  void initializeSprites(Graphics& graphics);
  void initializeSprite(Graphics& graphics, const SpriteState& sprite);
  SpriteState getSpriteState();

  bool onGround() const {return on_ground_;}

  float x_, y_;
  float acceleration_x_;
  float velocity_x_, velocity_y_;
  bool on_ground_;
  Jump jump_;

  HorizontalFacing horizontal_facing_;
  VerticalFacing vertical_facing_;

  std::map<SpriteState, std::unique_ptr<Sprite>> sprites_;
};


#endif //PLAYER_H_
