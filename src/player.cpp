#include "player.h"
#include "graphics.h"
#include "game.h"

#include <cmath> //yeet

namespace {
  const float kWalkingAcceleration = 0.0012f; // (pixels / ms) / ms
  const float kMaxSpeedX = 0.325f; // pixels / ms
  const float kMaxSpeedY = 0.325f;
  const float kSlowdownFactor = 0.8f;
  const float kJumpSpeed = 0.325f; // temp pixels / ms
  const float kGravity = 0.0012f;
  const int kJumpTime = 275; // ms
}

bool operator<(const Player::SpriteState& a, const Player::SpriteState& b)
{
  if(a.motion_type != b.motion_type)
  {
    return (a.motion_type < b.motion_type);
  }
  if(a.horizontal_facing != b.horizontal_facing)
  {
    return a.horizontal_facing < b.horizontal_facing;
  }

  //if both above checks, the two are both equal so we return false
  return false;
}

Player::Player(Graphics& graphics, float x, float y)
{
  x_ = x;
  y_ = y;

  velocity_x_ = 0.0f;
  velocity_y_ = 0.0f;
  acceleration_x_ = 0.0f;

  on_ground_ = false;

  horizontal_facing_ = RIGHT;

  initializeSprites(graphics);
}

void Player::initializeSprites(Graphics& graphics)
{
  //default: standing left
  sprites_[SpriteState()] = std::unique_ptr<Sprite>(
    new Sprite(graphics, "content/MyChar.bmp", 0, 0,
      Game::kTileSize, Game::kTileSize)
  );
  sprites_[SpriteState(WALKING, LEFT)] = std::unique_ptr<Sprite>(
    new AnimatedSprite(graphics, "content/MyChar.bmp", 0, 0,
      Game::kTileSize, Game::kTileSize, 15, 3)
  );

  sprites_[SpriteState(STANDING, RIGHT)] = std::unique_ptr<Sprite>(
    new Sprite(graphics, "content/MyChar.bmp", 0, Game::kTileSize,
      Game::kTileSize, Game::kTileSize)
  );
  sprites_[SpriteState(WALKING, RIGHT)] = std::unique_ptr<Sprite>(
    new AnimatedSprite(graphics, "content/MyChar.bmp", 0, Game::kTileSize,
      Game::kTileSize, Game::kTileSize, 15, 3)
  );
}

Player::SpriteState Player::getSpriteState()
{
  return SpriteState(acceleration_x_ == 0.0f ? STANDING : WALKING, horizontal_facing_);
}

void Player::update(int elapsed_time_ms)
{
  sprites_[getSpriteState()]->update(elapsed_time_ms);
  jump_.update(elapsed_time_ms);

  x_ += round(velocity_x_ * elapsed_time_ms);
  velocity_x_ += acceleration_x_ * elapsed_time_ms;
  if(acceleration_x_ < 0.0f) //moving left
  {
    velocity_x_ = std::max(velocity_x_, -kMaxSpeedX); //makes sure we don't go below negative max speed
    //so we don't move too fast
  }
  else if(acceleration_x_ > 0.0f) //moving right
  {
    velocity_x_ = std::min(velocity_x_, kMaxSpeedX);
  }
  else //not moving
  {
    velocity_x_ *= kSlowdownFactor;
  }

  y_ += round(velocity_y_ * elapsed_time_ms); //rounding in draw, might have to put it here
  if(jump_.active() == false)
  {
    velocity_y_ = std::min(velocity_y_ + (kGravity * elapsed_time_ms),
      kMaxSpeedY);
  }

  //TODO: proper detection
  if(y_ >= 320)
  {
    y_ = 320;
    velocity_y_ = 0.0f;
  }
  on_ground_ = y_ == 320;
}

void Player::draw(Graphics& graphics)
{
  sprites_[getSpriteState()]->draw(graphics, round(x_), round(y_));
}

void Player::startMovingLeft()
{
  acceleration_x_ = -kWalkingAcceleration;
  horizontal_facing_ = LEFT;
}

void Player::startMovingRight()
{
  acceleration_x_ = kWalkingAcceleration;
  horizontal_facing_ = RIGHT;
}

void Player::stopMoving()
{
  acceleration_x_ = 0.0f; //no acceleration for velocity slowdown
}

void Player::startJump()
{
  if(onGround()) // if we're on the ground: reset jump and give an initial velocity up
  {
    jump_.reset();
    velocity_y_ = -kJumpSpeed;
  }
  else if(velocity_y_ < 0.0f) //bc negative y = go up
  { // else, mid jump so re-activate anti gravity (non-Mario like)
    jump_.reactivate();
  }
}

void Player::stopJump()
{
  // let go of key, deactivate jump
  jump_.deactivate();
}

/**
Begin jump struct external implementations
*/
void Player::Jump::reset()
{
  time_remaining_ms_ = kJumpTime;
  reactivate();
}

void Player::Jump::update(int elapsed_time_ms)
{
  if(active_)
  {
    time_remaining_ms_ -= elapsed_time_ms;
    if(time_remaining_ms_ <= 0)
    {
      active_ = false;
    }
  }
}
/**
End jump struct external implementations
*/
