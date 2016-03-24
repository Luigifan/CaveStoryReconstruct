#include "player.h"
#include "graphics.h"
#include "game.h"
#include "map.h"

#include <cmath> //yeet
#include <cassert>
#include <algorithm>

namespace {
    const units::Velocity kMaxSpeedY = 0.325f; //pixels / ms
    const units::Acceleration kGravity = 0.0012f; //gravity

    //New constants for motion
    const units::Acceleration kJumpSpeed = 0.25f;
    const units::Acceleration kShortJumpSpeed = kJumpSpeed / 1.5f; //for when you take damage
    const units::Acceleration kAirAcceleration = 0.0003125f;
    const units::Acceleration kJumpGravity = 0.0003125f;

    const units::Acceleration kWalkingAcceleration = 0.00083007812f; // (pixels / ms) / ms
    const units::Velocity kMaxSpeedX = 0.15859375f; // pixels / ms
    const units::Acceleration kFriction = 0.00049804687f; // (pixels / ms) / ms
    //

    const units::Frame kJumpFrame = 1; //frame index for the jump frame
    const units::Frame kFallFrame = 2; //frame index for the fall frame
    const units::Frame kCharacterFrame = 0; //increase for various other styled quotes :) 0 is default quote
    const units::Frame kWalkFrame = 0; //frame index for the walk frame
    const units::Frame kStandFrame = 0; //frame index for standing frame
    const units::Frame kUpFrameOffset = 3; //index offset for if we're looking up
    const units::Frame kDownFrame = 6; //frame index for the looking down frame
    const units::Frame kBackFrame = 7; //frame index for looking back frame
    const units::FPS kWalkFps = 15; //frames per second for the walk animation. lower values will make it slower
    const units::Frame kNumFramesWalk = 3; //the number of frames in the walking animation

    //Hud
    const units::Game kHealthBarX = units::tileToGame(1);
    const units::Game kHealthBarY = units::tileToGame(2);
    const units::Game kHealthFillX = 5 * units::kHalfTile;
    const units::Game kHealthFillY = units::tileToGame(2);

    const units::Game kHealthBarSourceX = 0;
    const units::Game kHealthBarSourceY = 5 * units::kHalfTile;
    const units::Tile kHealthBarSourceWidth = 4;
    const units::Game kHealthBarSourceHeight = units::kHalfTile;

    const units::Game kHealthFillSourceX = 0;
    const units::Game kHealthFillSourceY = 3 * units::kHalfTile;
    const units::Game kHealthBarFillHeight = units::kHalfTile;
    //End HUD

    const std::string kSpriteFilePath = "content/MyChar.pbm";

    const Rectangle kCollisionX(6.0f,
        10.0f,
        20.0f,
        12.0f
        ); // x y w h
    const Rectangle kCollisionY(10.0f,
        2.0f,
        12.0f,
        30.0f
        );

    const units::MS kInvincibleFlashTime = 50; //every 50 ms, flash
    const units::MS kMaxInvincibleTime = 3000; //3 seconds

    struct CollisionInfo
    {
        bool collided;
        units::Tile row, column;
    };

    CollisionInfo getWallCollisionInfo(const Map& map, const Rectangle& rectangle)
    {
        CollisionInfo info = { false, 0, 0 }; //yay C++11
        auto tiles = map.getCollidingTiles(rectangle);
        // 5. Loop through checking wall tiles only. Tecccchnically
        for (size_t i = 0; i < tiles.size(); ++i)
        {
            if (tiles[i].tile_type_ == Map::WALL_TILE) //we can break and record that we've collided
            {
                info = { true, tiles[i].row, tiles[i].column };
                break;
            }
        }
        return info;
    };
}

bool operator<(const Player::SpriteState& a, const Player::SpriteState& b)
{
    if (a.motion_type != b.motion_type)
    {
        return (a.motion_type < b.motion_type);
    }
    if (a.horizontal_facing != b.horizontal_facing)
    {
        return a.horizontal_facing < b.horizontal_facing;
    }
    if (a.vertical_facing != b.vertical_facing)
    {
        return a.vertical_facing < b.vertical_facing;
    }

    //if both above checks, the two are both equal so we return false
    return false;
}

Player::Player(Graphics& graphics, units::Game x, units::Game y)
{
    x_ = x;
    y_ = y;

    velocity_x_ = 0.0f;
    velocity_y_ = 0.0f;
    acceleration_x_ = 0;

    on_ground_ = false;
    jump_active_ = false;
    interacting_ = false;

    invincible_ = false;
    invincible_time_ = 0;

    horizontal_facing_ = RIGHT;
    vertical_facing_ = HORIZONTAL;

    initializeSprites(graphics);
}

Rectangle Player::getDamageRect() const
{
    //combination of kCollisionX rect and kCollisionY rect at our x/y
    return Rectangle(x_ + kCollisionX.left(), y_ + kCollisionY.top(), kCollisionX.getWidth(), kCollisionY.getHeight());
}

void Player::initializeSprites(Graphics& graphics)
{
    health_bar_sprite_.reset(new Sprite(graphics, "content/TextBox.pbm",
        units::gameToPixel(kHealthBarSourceX), units::gameToPixel(kHealthBarSourceY),
        units::tileToPixel(kHealthBarSourceWidth),
        units::gameToPixel(kHealthBarSourceHeight)
    ));

    health_fill_sprite_.reset(new Sprite(graphics, "content/TextBox.pbm",
        units::gameToPixel(kHealthFillSourceX), units::gameToPixel(kHealthFillSourceY),
        units::gameToPixel(5 * units::kHalfTile - 2),
        units::gameToPixel(kHealthBarFillHeight)
    ));

    three_.reset(new Sprite(graphics, "content/TextBox.pbm",
        units::gameToPixel(3 * units::kHalfTile), units::gameToPixel(7 * units::kHalfTile),
        units::gameToPixel(units::kHalfTile), units::gameToPixel(units::kHalfTile)
    ));


    // for every motion type
      // for every horizontal facing
        // for every vertical facing
    // a lot of spritestates but hey
        // call initialize sprite for all of these with SpriteState
    for (int _motion = FIRST_MOTION_TYPE; _motion < LAST_MOTION_TYPE; _motion++)
    {
        for (int _hface = FIRST_HORIZONTAL_FACING; _hface < LAST_HORIZONTAL_FACING; _hface++)
        {
            for (int _vface = FIRST_VERTICAL_FACING; _vface < LAST_VERTICAL_FACING; _vface++)
            {
                initializeSprite(graphics, SpriteState((MotionType)_motion, (HorizontalFacing)_hface, (VerticalFacing)_vface));
            }
        }
    }
}

void Player::initializeSprite(Graphics& graphics, const SpriteState& sprite)
{
    units::Tile tile_x, tile_y;
    // tile_y will be..
      // frame * kTileSize 0 if going left
      // ((frame+ 1) * kTileSize) otherwise
    tile_y = sprite.horizontal_facing == LEFT ? kCharacterFrame : (1 + kCharacterFrame);
    //depends on vertical facing or horizontal motion type :////
    switch (sprite.motion_type)
    {
    case WALKING:
        tile_x = kWalkFrame;
        break;
    case STANDING:
        tile_x = kStandFrame;
        break;
    case JUMPING:
        tile_x = kJumpFrame;
        break;
    case FALLING:
        tile_x = kFallFrame;
        break;
    case INTERACTING:
        tile_x = kBackFrame;
        break;
    case LAST_MOTION_TYPE: //for completion's sake and to avoid annoying errors
        break;
    default:
        break;
    }
    tile_x = sprite.vertical_facing == UP ? tile_x = kUpFrameOffset : tile_x;

    if (sprite.motion_type == WALKING) //animated
    {
        sprites_[sprite] = std::shared_ptr<Sprite>(new AnimatedSprite(graphics,
            kSpriteFilePath,
            units::tileToPixel(tile_x), units::tileToPixel(tile_y),
            units::tileToPixel(1), units::tileToPixel(1),
            kWalkFps, kNumFramesWalk)
            );
    }
    else //static sprite, walking
    {
        //Only have a down frame if jumping or falling
        if (sprite.vertical_facing == DOWN && (sprite.motion_type == JUMPING || sprite.motion_type == FALLING))
        {
            tile_x = kDownFrame;
        }
        sprites_[sprite] = std::shared_ptr<Sprite>(new Sprite(graphics,
            kSpriteFilePath,
            units::tileToPixel(tile_x), units::tileToPixel(tile_y),
            units::tileToPixel(1), units::tileToPixel(1))
            );
    }
}

Player::SpriteState Player::getSpriteState()
{
    MotionType motion;
    if (interacting_)
    {
        motion = INTERACTING;
    }
    else if (onGround())
    {
        motion = acceleration_x_ == 0 ? STANDING : WALKING;
    }
    else //in the air
    {
        motion = velocity_y_ < 0.0f ? JUMPING : FALLING;
    }
    return SpriteState(motion, horizontal_facing_, vertical_facing_);
}

void Player::update(units::MS elapsed_time_ms, const Map& map)
{
    sprites_[getSpriteState()]->update(elapsed_time_ms);

    if (invincible_)
    {
        invincible_time_ += elapsed_time_ms;
        invincible_ = invincible_time_ < kMaxInvincibleTime;
    }

    updateX(elapsed_time_ms, map);
    updateY(elapsed_time_ms, map);
}

void Player::updateX(units::MS elapsed_time_ms, const Map& map)
{
    // 1. Update velocity
    units::Acceleration acceleration_x_actual = 0.0f;
    if (acceleration_x_ < 0) //left
        acceleration_x_actual = on_ground_ ? -kWalkingAcceleration : -kAirAcceleration;
    else if (acceleration_x_ > 0) //right
        acceleration_x_actual = on_ground_ ? kWalkingAcceleration : kAirAcceleration;

    velocity_x_ += acceleration_x_actual * elapsed_time_ms;

    if (acceleration_x_ < 0) //moving left
    {
        velocity_x_ = std::max(velocity_x_, -kMaxSpeedX); //makes sure we don't go below negative max speed
        //so we don't move too fast
    }
    else if (acceleration_x_ > 0) //moving right
    {
        velocity_x_ = std::min(velocity_x_, kMaxSpeedX);
    }
    else if (onGround())//not moving
    {
        /**
        Friction opposes motion.
        Here, we check the direction.
          If we're going to the right, we want the max of either 0 or or velocity - friction * deltaTime. Why? Because friction opposes motion.
          Same for left except with max because we're dealing with negative values.
        */

        velocity_x_ = velocity_x_ > 0.0f ? 
            std::max(0.0f, velocity_x_ - kFriction * elapsed_time_ms) : 
            std::min(0.0f, velocity_x_ + kFriction * elapsed_time_ms);
    }

    // 2. Calculate delta x
    const units::Game delta = velocity_x_ * elapsed_time_ms;

    // 3. Check collision in delta direction
    if (delta > 0.0f)
    {
        // 4. Check collision for down direction delta.
        CollisionInfo info = getWallCollisionInfo(map, rightCollision(delta));
        // 6. React
        if (info.collided)
        {
            // Alignment and stopping velocity so we don't move anymore.
            x_ = units::tileToGame(info.column) - kCollisionX.right();
            velocity_x_ = 0.0f; //so we don't keep moving
        }
        else
        {
            x_ += delta;
        }

        //Verify left direction
        info = { false,0,0 };
        info = getWallCollisionInfo(map, leftCollision(0));
        if (info.collided)
        {
            x_ = units::tileToGame(info.column) + kCollisionX.right();
        }
    }
    else // moving left
    {
        // 4. Check collision for down direction delta.
        CollisionInfo info = getWallCollisionInfo(map, leftCollision(delta));
        // 6. React
        if (info.collided)
        {
            // Alignment and stopping velocity so we don't move anymore.
            x_ = units::tileToGame(info.column) + kCollisionX.right();
            velocity_x_ = 0.0f; //so we don't keep moving
        }
        else
        {
            x_ += delta;
        }

        //Verify left direction
        info = { false,0,0 };
        info = getWallCollisionInfo(map, rightCollision(0));
        if (info.collided)
        {
            x_ = units::tileToGame(info.column) - kCollisionX.right();
        }
    }
}

void Player::updateY(units::MS elapsed_time_ms, const Map& map)
{
    // 1. Update velocity
    const units::Acceleration gravity = jump_active_ && velocity_y_ < 0.0f ? kJumpGravity : kGravity; //holding jump button

    velocity_y_ = std::min(velocity_y_ + gravity * elapsed_time_ms,
        kMaxSpeedY);

    // 2. Calculate delta movement for collision
    const units::Game delta = velocity_y_ * elapsed_time_ms;

    // 3. Check collection for direction of delta (up/down)
    if (delta > 0) //going down
    {
        // 4. Check collision for down direction delta.
        CollisionInfo info = getWallCollisionInfo(map, bottomCollision(delta));
        // 6. React
        if (info.collided)
        {
            // Alignment and stopping velocity so we don't move anymore.
            y_ = units::tileToGame(info.row) - kCollisionY.bottom();
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
        info = { false,0,0 };
        info = getWallCollisionInfo(map, topCollision(0));
        if (info.collided)
        {
            y_ = units::tileToGame(info.row) + kCollisionY.getHeight();
            //on_ground_ = true;
        }
    }
    else //jumping
    {
        // 4. Check collision for down direction delta.
        CollisionInfo info = getWallCollisionInfo(map, topCollision(delta));
        // 6. React
        if (info.collided)
        {
            // Alignment and stopping velocity so we don't move anymore.
            y_ = units::tileToGame(info.row) + kCollisionY.getHeight();
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
        info = { false,0,0 };
        info = getWallCollisionInfo(map, bottomCollision(0));
        if (info.collided)
        {
            y_ = units::tileToGame(info.row) - kCollisionY.bottom();
            on_ground_ = true;
        }
    }
}

void Player::draw(Graphics& graphics)
{
    //Dividing to break into chunks of 50ms
    // %2 == 0; 1 part invisible and part visible
    // %3 == 0; 1 part invisible and 2 parts visible
    if (spriteIsVisible())
    {
        sprites_[getSpriteState()]->draw(graphics, x_, y_);
    }
}

void Player::drawHUD(Graphics& graphics) const
{
    if (spriteIsVisible())
    {
        health_bar_sprite_->draw(graphics, kHealthBarX, kHealthBarY);
        health_fill_sprite_->draw(graphics, kHealthFillX, kHealthFillY);

        three_->draw(graphics, units::tileToGame(2), units::tileToGame(2));
    }
}

bool Player::spriteIsVisible() const
{
    return !(invincible_ && invincible_time_ / kInvincibleFlashTime % 2 == 0);
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
    if (vertical_facing_ == DOWN) return;

    vertical_facing_ = DOWN;

    if (onGround())
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
    if (onGround()) // if we're on the ground: reset jump and give an initial velocity up
    {
        velocity_y_ = -kJumpSpeed;
    }
    else if (velocity_y_ < 0.0f) //bc negative y = go up
    { // else, mid jump so re-activate anti gravity (non-Mario like)
        jump_active_ = true;
    }
}

void Player::stopJump()
{
    // let go of key, deactivate jump
    jump_active_ = false;
}

void Player::takeDamage()
{
    if (invincible_) return;

    //also, if we're already jumping we won't decrease our jump speed
    velocity_y_ = std::min(velocity_y_, -kShortJumpSpeed); //we can't repeatedly set this because we'll keep flying up

    invincible_ = true;
    invincible_time_ = 0;
}

Rectangle Player::leftCollision(units::Game delta) const // left x
{
    //delta is a prediction of where we're going to go

    //assert is basically a "make sure this is okay if not, abort"
    assert(delta <= 0); //left collision if delta is negative or 0
    return Rectangle(
        x_ + kCollisionX.left() + delta,
        y_ + kCollisionX.top(),
        kCollisionX.getWidth() / 2 - delta, //lob off half bc we can, taking into account our delta 
        kCollisionX.getHeight()
    );
}

Rectangle Player::rightCollision(units::Game delta) const // right x
{
    assert(delta >= 0);
    return Rectangle(
        x_ + kCollisionX.left() + (kCollisionX.getWidth() / 2),
        y_ + kCollisionX.top(),
        kCollisionX.getWidth() / 2 + delta,
        kCollisionX.getHeight()
        );
}

Rectangle Player::bottomCollision(units::Game delta) const // bottom y
{
    assert(delta >= 0);
    return Rectangle(
        x_ + kCollisionY.left(),
        y_ + kCollisionY.top() + kCollisionY.getHeight() / 2,
        kCollisionY.getWidth(),
        kCollisionY.getHeight() / 2 + delta
        );
}

Rectangle Player::topCollision(units::Game delta) const // top y
{
    assert(delta <= 0);
    return Rectangle(
        x_ + kCollisionY.left(),
        y_ + kCollisionY.top() + delta,
        kCollisionY.getWidth(),
        kCollisionY.getHeight() / 2 - delta //lob off half bc we can, taking into account our delta 
        );
}