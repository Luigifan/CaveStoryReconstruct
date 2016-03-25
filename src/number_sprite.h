#ifndef NUMBER_SPRITE_H_
#define NUMBER_SPRITE_H_

#include <memory>
#include <vector>
#include <cassert>
#include "unit.h"

struct Sprite;
struct Graphics;

struct NumberSprite
{
    NumberSprite(Graphics& graphics, int number, int num_digits=0); //0 means we don't give a shit
    void draw(Graphics& graphics, units::Game x, units::Game y);
private:
    units::Game padding_;
    std::vector<std::shared_ptr<Sprite> > reverse_digits_;
};

#endif NUMBER_SPRITE_H_