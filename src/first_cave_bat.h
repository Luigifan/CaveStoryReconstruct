#ifndef FIRST_CAVE_BAT_H
#define FIRST_CAVE_BAT_H

#include <memory>
#include "unit.h"

struct Sprite;
struct Graphics;

struct FirstCaveBat 
{
	FirstCaveBat(Graphics& graphics, units::Game x, units::Game y);
	void draw(Graphics& graphics) const;
	void update(units::MS elapsed_time_ms) const;
private:
	std::shared_ptr<Sprite> sprite_;
	units::Game x_, y_;
};

#endif //FIRST_CAVE_BAT_H