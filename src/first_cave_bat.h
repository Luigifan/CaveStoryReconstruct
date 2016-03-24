#ifndef FIRST_CAVE_BAT_H
#define FIRST_CAVE_BAT_H

#include <memory>
#include <map>
#include "unit.h"
#include "rectangle.h"

struct Sprite;
struct Graphics;

struct FirstCaveBat 
{
	FirstCaveBat(Graphics& graphics, units::Game x, units::Game y);
	void draw(Graphics& graphics);
	void update(units::MS elapsed_time_ms, units::Game player_x);

	Rectangle damageRectangle() const
	{
		//single point
		return Rectangle(x_ + units::kHalfTile, y_ + units::kHalfTile, 0, 0);
	}

private:
	enum Facing {
		LEFT = 0,
		RIGHT = 1,
		LAST_FACING = 2
	};
	struct SpriteState 
	{
		SpriteState(Facing facing) : facing(facing){}
		Facing facing;
	};
	friend bool operator<(const SpriteState& a, const SpriteState& b)
	{return a.facing < b.facing;}
	void initializeSprites(Graphics& graphics);
	void initializeSprite(Graphics& graphics, const SpriteState& sprite);
	SpriteState getSpriteState() const;

	std::map<SpriteState, std::shared_ptr<Sprite>> sprite_map_;
	units::Game x_, y_;
	const units::Game center_y_;
	units::Degrees flight_angle_;
	Facing facing_;
};

/**
We could add a simple friendly bool once we start colliding to check if we need to damage/truly collide or not
*/

#endif //FIRST_CAVE_BAT_H