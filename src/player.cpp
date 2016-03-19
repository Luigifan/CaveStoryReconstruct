#include "player.h"
#include "graphics.h"
#include "game.h"

#include <cmath> //yeet

namespace {
  const float kWalkingAcceleration = 0.0012f; // (pixels / ms) / ms
  const float kMaxSpeedX = 0.325f; //pixels / ms
  const float kMaxSpeedY = 0.325f; //pixels / ms
  const float kSlowdownFactor = 0.8f; //Slowdown factor
  const float kJumpSpeed = 0.325f; //temp pixels / ms
  const float kGravity = 0.0012f; //gravity

  const int kJumpTime = 275; // ms
  const int kJumpFrame = 1; //frame index for the jump frame
  const int kFallFrame = 2; //frame index for the fall frame
  const int kCharacterFrame = 0; //increase for various other styled quotes :)
  const int kWalkFrame = 0; //frame index for the walk frame
  const int kStandFrame = 0; //frame index for standing frame
  const int kUpFrameOffset = 3; //index offset for if we're looking up
  const int kDownFrame = 6; //frame index for the looking down frame
  const int kBackFrame = 7; //frame index for looking back frame
  const int kWalkFps = 15; //frames per second for the walk animation. lower values will make it slower
  const int kNumFramesWalk = 3; //the number of frames in the walking animation

  const std::string kSpriteFilePath = "content/MyChar.bmp";
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
  if(a.vertical_facing != b.vertical_facing)
  {
    return a.vertical_facing < b.vertical_facing;
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
  vertical_facing_ = HORIZONTAL;

  initializeSprites(graphics);
}

void Player::initializeSprites(Graphics& graphics)
{
  // for every motion type
    // for every horizontal facing
      // for every vertical facing
  // a lot of spritestates but hey
      // call initialize sprite for all of these with SpriteState
  for(int _motion = FIRST_MOTION_TYPE ; _motion < LAST_MOTION_TYPE; _motion++)
  {
    for(int _hface = FIRST_HORIZONTAL_FACING; _hface < LAST_HORIZONTAL_FACING; _hface++)
    {
      for(int _vface = FIRST_VERTICAL_FACING; _vface < LAST_VERTICAL_FACING; _vface++)
      {
        initializeSprite(graphics, SpriteState((MotionType)_motion, (HorizontalFacing)_hface, (VerticalFacing)_vface));
      }
    }
  }
}

void Player::initializeSprite(Graphics& graphics, const SpriteState& sprite)
{
  int source_x, source_y;
  // source_y will be..
    // frame * kTileSize 0 if going left
    // ((frame+ 1) * kTileSize) otherwise
  source_y = sprite.horizontal_facing == LEFT ? kCharacterFrame * Game::kTileSize : (1 + kCharacterFrame) * Game::kTileSize;
  //depends on vertical facing or horizontal motion type :////
  switch(sprite.motion_type)
  {
    case WALKING:
      source_x = kWalkFrame * Game::kTileSize;
      break;
    case STANDING:
      source_x = kStandFrame * Game::kTileSize;
      break;
    case JUMPING:
      source_x = kJumpFrame * Game::kTileSize;
      break;
    case FALLING:
      source_x = kFallFrame * Game::kTileSize;
      break;
    case LAST_MOTION_TYPE: //for completion's sake and to avoid annoying errors
      break;
    default:
      break;
  }
  source_x = sprite.vertical_facing == UP ? source_x = kUpFrameOffset * Game::kTileSize : source_x;

  if(sprite.motion_type == WALKING) //animated
  {
    sprites_[sprite] = std::unique_ptr<Sprite>(new AnimatedSprite(graphics,
      kSpriteFilePath,
      source_x, source_y,
      Game::kTileSize, Game::kTileSize,
      kWalkFps, kNumFramesWalk)
    );
  }
  else //static sprite, walking
  {
    if(sprite.vertical_facing == DOWN)
    {
      source_x = sprite.motion_type == STANDING ? kBackFrame * Game::kTileSize : kDownFrame * Game::kTileSize;
    }
    sprites_[sprite] = std::unique_ptr<Sprite>(new Sprite(graphics,
      kSpriteFilePath,
      source_x, source_y,
      Game::kTileSize, Game::kTileSize)
    );
  }
}

Player::SpriteState Player::getSpriteState()
{
  MotionType motion;
  if(onGround())
  {
    motion = acceleration_x_ == 0.0f ? STANDING : WALKING;
  }
  else //in the air
  {
    motion = velocity_y_ < 0.0f ? JUMPING : FALLING;
  }
  return SpriteState(motion, horizontal_facing_, vertical_facing_);
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
  else if(onGround())//not moving
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

void Player::lookUp()
{
  vertical_facing_ = UP;
}

void Player::lookDown()
{
  vertical_facing_ = DOWN;
}

void Player::lookHorizontal()
{
  vertical_facing_ = HORIZONTAL;
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
