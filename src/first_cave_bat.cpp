#include "first_cave_bat.h"
#include "animated_sprite.h"
#include "graphics.h"

namespace {
	const units::Frame kNumFlyFrames = 3;
	const units::FPS kFlyFps = 10;
}

FirstCaveBat::FirstCaveBat(Graphics& graphics, units::Game x, units::Game y)
{
	x_ = x;
	y_ = y;

	sprite_.reset(new AnimatedSprite(
		graphics,
		"content/Npc/NpcCemet.pbm",
		units::tileToPixel(2),
		units::tileToPixel(2),
		units::tileToPixel(1),
		units::tileToPixel(1),
		kFlyFps,
		kNumFlyFrames
	));
}

void FirstCaveBat::draw(Graphics& graphics) const
{
	sprite_->draw(graphics, x_, y_);
}

void FirstCaveBat::update(units::MS elapsed_time_ms) const
{
	sprite_->update(elapsed_time_ms);
}
