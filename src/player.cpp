#include "player.h"
#include "graphics.h"
#include "game.h"

#include <cmath> //yeet

namespace {
  const float kWalkingAcceleration = 0.0012f; // (pixels / ms) / ms
  const float kMaxSpeedX = 0.325f; // pixels / ms
  const float kSlowdownFactor = 0.8f;
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
  acceleration_x_ = 0.0f;

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
