#ifndef PLAYER_H_
#define PLAYER_H_

#include <memory>
#include <map>
#include "animated_sprite.h"
#include "rectangle.h"
#include "unit.h"

struct Graphics;
struct Map;

struct Player
{
  Player(Graphics& graphics, units::Game x, units::Game y);

  //Update velocity, calculate delta, check collision, react, repeat.
  void update(units::MS elapsed_time_ms, const Map& map);
  void draw(Graphics& graphics);

  void startMovingLeft();
  void startMovingRight();
  void stopMoving();

  void lookUp();
  void lookDown();
  void lookHorizontal();

  void startJump();
  void stopJump();

  Rectangle getDamageRect() const;

  units::Game getCenterX() const
  {
	  return x_ + units::tileToGame(1) / 2.0f;
  }
private:
  enum MotionType : int{
    FIRST_MOTION_TYPE = 0,
    STANDING = 0, WALKING = 1, JUMPING = 2, FALLING = 3, INTERACTING = 4,
    LAST_MOTION_TYPE = 5
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
  //friend because SpriteState is a private friend class of player
  friend bool operator<(const SpriteState& a, const SpriteState& b);
  void initializeSprites(Graphics& graphics);
  void initializeSprite(Graphics& graphics, const SpriteState& sprite);

  SpriteState getSpriteState();

  Rectangle leftCollision(units::Game delta) const; // left x
  Rectangle rightCollision(units::Game delta) const; // right x
  Rectangle bottomCollision(units::Game delta) const; // bottom y
  Rectangle topCollision(units::Game delta) const; // top y

  void updateX(units::MS elapsed_time_ms, const Map& map);
  void updateY(units::MS elapsed_time_ms, const Map& map);

  bool onGround() const {return on_ground_;}

  units::Game x_, y_;
  /*special acceleration*/int acceleration_x_; //+, -, 0
  units::Velocity velocity_x_, velocity_y_;
  bool on_ground_;
  bool jump_active_;
  bool interacting_;

  HorizontalFacing horizontal_facing_;
  VerticalFacing vertical_facing_;

  std::map<SpriteState, std::shared_ptr<Sprite>> sprites_;
};


#endif //PLAYER_H_
