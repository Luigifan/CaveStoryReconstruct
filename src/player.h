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
  enum MotionType{
    FIRST_MOTION_TYPE,
    STANDING = FIRST_MOTION_TYPE, WALKING, JUMPING, FALLING, 
    LAST_MOTION_TYPE
  };
  enum HorizontalFacing {
    FIRST_HORIZONTAL_FACING,
    LEFT = FIRST_HORIZONTAL_FACING, RIGHT,
    LAST_HORIZONTAL_FACING
  };
  enum VerticalFacing{
    FIRST_VERTICAL_FACING,
    UP = FIRST_VERTICAL_FACING, DOWN, HORIZONTAL,
    LAST_VERTICAL_FACING
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
