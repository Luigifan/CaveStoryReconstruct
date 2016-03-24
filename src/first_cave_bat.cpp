#include "first_cave_bat.h"
#include "animated_sprite.h"
#include "graphics.h"

/**
The cave bat's height will be a function of time.
y = sin(time);
*/

namespace {
	const units::Frame kNumFlyFrames = 3;
	const units::FPS kFlyFps = 13;

	const units::AngularVelocity kAngularVelocity = 120.0f / 1000.0f; //120 degrees / 1 second.
}

FirstCaveBat::FirstCaveBat(Graphics& graphics, units::Game x, units::Game y)
{
	x_ = x;
	y_ = y;
	flight_angle_ = 0.0f;
	facing_ = LEFT;

	initializeSprites(graphics);
}

void FirstCaveBat::draw(Graphics& graphics)
{
	//keep y constant but use it as part of our sine movement
	//								5 units total		   amplitude of 2
	const units::Game draw_y = y_ + units::tileToGame(5) / 2.0f * std::sin(units::degreesToRadians(flight_angle_));
	sprite_map_[getSpriteState()]->draw(graphics, x_, draw_y);
}

void FirstCaveBat::update(units::MS elapsed_time_ms, units::Game player_x)
{
	flight_angle_ += kAngularVelocity * elapsed_time_ms;

	facing_ = x_ + units::tileToGame(1) / 2.0f > player_x ? LEFT : RIGHT; //if our x is less than player's x, face right

	sprite_map_[getSpriteState()]->update(elapsed_time_ms);
}

void FirstCaveBat::initializeSprites(Graphics& graphics)
{
	for (int i = LEFT; i < (int)LAST_FACING; i++)
	{
		initializeSprite(graphics, SpriteState(Facing(i)));
	}
}

void FirstCaveBat::initializeSprite(Graphics& graphics, const SpriteState& sprite)
{
	//facing right = tile_y = 3
	//else, 2
	units::Tile tile_y = sprite.facing == RIGHT ? 3 : 2;

	sprite_map_[sprite].reset(new AnimatedSprite(
		graphics,
		"content/Npc/NpcCemet.pbm",
		units::tileToPixel(2),
		units::tileToPixel(tile_y),
		units::tileToPixel(1),
		units::tileToPixel(1),
		kFlyFps,
		kNumFlyFrames
	));
}

FirstCaveBat::SpriteState FirstCaveBat::getSpriteState() const
{
	return SpriteState(facing_);
}