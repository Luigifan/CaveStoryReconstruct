#include "player.h"
#include "graphics.h"
#include "game.h"

#include <cmath> //yeet

namespace {
  const float kWalkingAcceleration = 0.0012f; // (pixels / ms) / ms
  const float kMaxSpeedX = 0.325f; // pixels / ms
  const float kSlowdownFactor = 0.8f;
}

Player::Player(Graphics& graphics, int x, int y)
{
  x_ = x;
  y_ = y;

  velocity_x_ = 0.0f;
  acceleration_x_ = 0.0f;
  sprite_.reset(graphics.createAnimatedSprite("content/MyChar.bmp", 0, 0, Game::kTileSize, Game::kTileSize, 15, 3));
}

void Player::update(int elapsed_time_ms)
{
  sprite_->update(elapsed_time_ms);

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
}

void Player::draw(Graphics& graphics)
{
  sprite_->draw(graphics, x_, y_);
}

void Player::startMovingLeft()
{
  acceleration_x_ = -kWalkingAcceleration;
}

void Player::startMovingRight()
{
  acceleration_x_ = kWalkingAcceleration;
}

void Player::stopMoving()
{
  acceleration_x_ = 0.0f; //no acceleration for velocity slowdown
}
