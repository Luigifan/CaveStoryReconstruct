#include "player.h"
#include "graphics.h"
#include "game.h"
#include "map.h"

#include <cmath> //yeet
#include <cassert>

namespace {
  const float kMaxSpeedY = 0.325f; //pixels / ms
  const float kGravity = 0.0012f; //gravity

  //New constants for motion
  const float kJumpSpeed = 0.25f;
  const float kAirAcceleration = 0.0003125f;
  const float kJumpGravity = 0.0003125f;

  const float kWalkingAcceleration = 0.0083007812f; // (pixels / ms) / ms
  const float kMaxSpeedX = 0.15859375f; // pixels / ms
  const float kFriction = 0x0049804687; // (pixels / ms) / ms
  //

  const int kJumpFrame = 1; //frame index for the jump frame
  const int kFallFrame = 2; //frame index for the fall frame
  const int kCharacterFrame = 4; //increase for various other styled quotes :) 0 is default quote
  const int kWalkFrame = 0; //frame index for the walk frame
  const int kStandFrame = 0; //frame index for standing frame
  const int kUpFrameOffset = 3; //index offset for if we're looking up
  const int kDownFrame = 6; //frame index for the looking down frame
  const int kBackFrame = 7; //frame index for looking back frame
  const int kWalkFps = 15; //frames per second for the walk animation. lower values will make it slower
  const int kNumFramesWalk = 3; //the number of frames in the walking animation

  const std::string kSpriteFilePath = "content/MyChar.bmp";

  const Rectangle kCollisionX(6, 10, 20, 12); // x y w h
  const Rectangle kCollisionY(10, 2, 12, 30);

  struct CollisionInfo
  {
      bool collided;
      int row, column;
  };

  CollisionInfo getWallCollisionInfo(const Map& map, const Rectangle& rectangle)
  {
    CollisionInfo info = {false, 0, 0}; //yay C++11
    auto tiles = map.getCollidingTiles(rectangle);
    // 5. Loop through checking wall tiles only. Tecccchnically
    for(size_t i = 0; i < tiles.size(); ++i)
    {
      if(tiles[i].tile_type_ == Map::WALL_TILE) //we can break and record that we've collided
      {
        info = {true, tiles[i].row, tiles[i].column};
        break;
      }
    }
    return info;
  };
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
  acceleration_x_ = 0;

  on_ground_ = false;
  jump_active_ = false;
  interacting_ = false;

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
    case INTERACTING:
      source_x = kBackFrame * Game::kTileSize;
      break;
    case LAST_MOTION_TYPE: //for completion's sake and to avoid annoying errors
      break;
    default:
      break;
  }
  source_x = sprite.vertical_facing == UP ? source_x = kUpFrameOffset * Game::kTileSize : source_x;

  if(sprite.motion_type == WALKING) //animated
  {
    sprites_[sprite] = std::shared_ptr<Sprite>(new AnimatedSprite(graphics,
      kSpriteFilePath,
      source_x, source_y,
      Game::kTileSize, Game::kTileSize,
      kWalkFps, kNumFramesWalk)
    );
  }
  else //static sprite, walking
  {
    //Only have a down frame if jumping or falling
    if(sprite.vertical_facing == DOWN && (sprite.motion_type == JUMPING || sprite.motion_type == FALLING))
    {
      source_x = kDownFrame * Game::kTileSize;
    }
    sprites_[sprite] = std::shared_ptr<Sprite>(new Sprite(graphics,
      kSpriteFilePath,
      source_x, source_y,
      Game::kTileSize, Game::kTileSize)
    );
  }
}

Player::SpriteState Player::getSpriteState()
{
  MotionType motion;
  if(interacting_)
  {
    motion = INTERACTING;
  }
  else if(onGround())
  {
    motion = acceleration_x_ == 0 ? STANDING : WALKING;
  }
  else //in the air
  {
    motion = velocity_y_ < 0.0f ? JUMPING : FALLING;
  }
  return SpriteState(motion, horizontal_facing_, vertical_facing_);
}

void Player::update(int elapsed_time_ms, const Map& map)
{
  sprites_[getSpriteState()]->update(elapsed_time_ms);

  updateX(elapsed_time_ms, map);
  updateY(elapsed_time_ms, map);
}

void Player::updateX(int elapsed_time_ms, const Map& map)
{
  // 1. Update velocity
  float acceleration_x_actual = 0.0f;
  if(acceleration_x_ < 0) //left
    acceleration_x_actual = on_ground_ ? -kWalkingAcceleration : -kAirAcceleration;
  else if(acceleration_x_ > 0) //right
    acceleration_x_actual = on_ground_ ? kWalkingAcceleration : kAirAcceleration;

  velocity_x_ += acceleration_x_actual * elapsed_time_ms;

  if(acceleration_x_ < 0) //moving left
  {
    velocity_x_ = std::max(velocity_x_, -kMaxSpeedX); //makes sure we don't go below negative max speed
    //so we don't move too fast
  }
  else if(acceleration_x_ > 0) //moving right
  {
    velocity_x_ = std::min(velocity_x_, kMaxSpeedX);
  }
  else if(onGround())//not moving
  {
    /**
    Friction opposes motion.
    Here, we check the direction.
      If we're going to the right, we want the max of either 0 or or velocity - friction * deltaTime. Why? Because friction opposes motion.
      Same for left except with max because we're dealing with negative values.
    */
    
    velocity_x_ = velocity_x_ > 0.0f ? std::max(0.0f, velocity_x_ - kFriction * elapsed_time_ms)
      : std::min(0.0f, velocity_x_ + kFriction * elapsed_time_ms);
  }

  // 2. Calculate delta x
  const int delta = round(velocity_x_ * elapsed_time_ms);

  // 3. Check collision in delta direction
  if(delta > 0)
  {
    // 4. Check collision for down direction delta.
    CollisionInfo info = getWallCollisionInfo(map, rightCollision(delta));
    // 6. React
    if(info.collided)
    {
      // Alignment and stopping velocity so we don't move anymore.
      x_ = info.column * Game::kTileSize - kCollisionX.right();
      velocity_x_ = 0.0f; //so we don't keep moving
    }
    else
    {
      x_ += delta;
    }

    //Verify left direction
    info = {false,0,0};
    info = getWallCollisionInfo(map, leftCollision(0));
    if(info.collided)
    {
      x_ = info.column * Game::kTileSize + kCollisionX.right();
    }
  }
  else // moving left
  {
    // 4. Check collision for down direction delta.
    CollisionInfo info = getWallCollisionInfo(map, leftCollision(delta));
    // 6. React
    if(info.collided)
    {
      // Alignment and stopping velocity so we don't move anymore.
      x_ = info.column * Game::kTileSize + kCollisionX.right();
      velocity_x_ = 0.0f; //so we don't keep moving
    }
    else
    {
      x_ += delta;
    }

    //Verify left direction
    info = {false,0,0};
    info = getWallCollisionInfo(map, rightCollision(0));
    if(info.collided)
    {
      x_ = info.column * Game::kTileSize - kCollisionX.right();
    } 
  }
}

void Player::updateY(int elapsed_time_ms, const Map& map)
{
  // 1. Update velocity
  const float gravity = jump_active_ && velocity_y_ < 0.0f ? kJumpGravity : kGravity; //holding jump button
  
    velocity_y_ = std::min(velocity_y_ + gravity * elapsed_time_ms,
      kMaxSpeedY);

  // 2. Calculate delta movement for collision
  const int delta = round(velocity_y_ * elapsed_time_ms);

  // 3. Check collection for direction of delta (up/down)
  if(delta > 0) //going down
  {
    // 4. Check collision for down direction delta.
    CollisionInfo info = getWallCollisionInfo(map, bottomCollision(delta));
    // 6. React
    if(info.collided)
    {
      // Alignment and stopping velocity so we don't move anymore.
      y_ = info.row * Game::kTileSize - kCollisionY.bottom();
      velocity_y_ = 0.0f;
      // we could set this to negative to bounce o:
      on_ground_ = true;
    }
    else
    {
      y_ += delta;
      on_ground_ = false;
    }

    //Verify up direction
    info = {false,0,0};
    info = getWallCollisionInfo(map, topCollision(0));
    if(info.collided)
    {
      y_ = info.row * Game::kTileSize + kCollisionY.getHeight();
      //on_ground_ = true;
    }
  }
  else //jumping
  {
    // 4. Check collision for down direction delta.
    CollisionInfo info = getWallCollisionInfo(map, topCollision(delta));
    // 6. React
    if(info.collided)
    {
      // Alignment and stopping velocity so we don't move anymore.
      y_ = info.row * Game::kTileSize + kCollisionY.getHeight();
      velocity_y_ = 0.0f;
      // we could set this to negative to bounce o:
      on_ground_ = false;
    }
    else
    {
      y_ += delta;
      on_ground_ = false;
    }

    //Verify up direction
    info = {false,0,0};
    info = getWallCollisionInfo(map, bottomCollision(0));
    if(info.collided)
    {
      y_ = info.row * Game::kTileSize - kCollisionY.bottom();
      on_ground_ = true;
    }
  }
}

void Player::draw(Graphics& graphics)
{
  sprites_[getSpriteState()]->draw(graphics, x_, y_);
}

void Player::startMovingLeft()
{
  interacting_ = false;
  acceleration_x_ = -1;
  horizontal_facing_ = LEFT;
}

void Player::startMovingRight()
{
  interacting_ = false;
  acceleration_x_ = 1;
  horizontal_facing_ = RIGHT;
}

void Player::stopMoving()
{
  acceleration_x_ = 0; //no acceleration for velocity slowdown
}

void Player::lookUp()
{
  interacting_ = false;
  vertical_facing_ = UP;
}

void Player::lookDown()
{
  //won't set interacting if coming down or looking down.
  if(vertical_facing_ == DOWN) return;

  vertical_facing_ = DOWN;

  if(onGround())
    interacting_ = true;
}

void Player::lookHorizontal()
{
  vertical_facing_ = HORIZONTAL;
}

void Player::startJump()
{
  interacting_ = false;
  jump_active_ = true;
  if(onGround()) // if we're on the ground: reset jump and give an initial velocity up
  {
    velocity_y_ = -kJumpSpeed;
  }
  else if(velocity_y_ < 0.0f) //bc negative y = go up
  { // else, mid jump so re-activate anti gravity (non-Mario like)
    jump_active_ = true;
  }
}

void Player::stopJump()
{
  // let go of key, deactivate jump
  jump_active_ = false;
}

Rectangle Player::leftCollision(int delta) const // left x
{
  //delta is a prediction of where we're going to go

  //assert is basically a "make sure this is okay if not, abort"
  assert(delta <= 0); //left collision if delta is negative or 0
  return Rectangle (
    x_ + kCollisionX.left() + delta, 
    y_ + kCollisionX.top(),
    (kCollisionX.getWidth() / 2) - delta, //lob off half bc we can, taking into account our delta 
    kCollisionX.getHeight()
  );
} 

Rectangle Player::rightCollision(int delta) const // right x
{
  assert(delta >= 0);
  return Rectangle (
    x_ + kCollisionX.left() + (kCollisionX.getWidth() / 2),
    y_ + kCollisionX.top(),
    kCollisionX.getWidth() / 2 + delta,
    kCollisionX.getHeight()
  );
}

Rectangle Player::bottomCollision(int delta) const // bottom y
{
  assert(delta >= 0);
  return Rectangle (
    x_ + kCollisionY.left(),
    y_ + kCollisionY.top() + kCollisionY.getHeight() / 2,
    kCollisionY.getWidth(),
    kCollisionY.getHeight() / 2 + delta
  );
} 

Rectangle Player::topCollision(int delta) const // top y
{
  assert(delta <= 0);
  return Rectangle (
    x_ + kCollisionY.left(), 
    y_ + kCollisionY.top() + delta,
    kCollisionY.getWidth(), 
    kCollisionY.getHeight() / 2 - delta //lob off half bc we can, taking into account our delta 
  );
} 