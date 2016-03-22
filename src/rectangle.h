#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "unit.h"

struct Rectangle
{
	Rectangle(units::Game x, units::Game y, units::Game width, units::Game height) : x_(x), y_(y), w_(width), h_(height)
	{	}

	units::Game left() const {return x_;}
	units::Game right() const {return x_ + w_;}
	units::Game top() const {return y_;}
	units::Game bottom() const {return y_ + h_;}

	units::Game getWidth() const {return w_;}
	units::Game getHeight() const {return h_;}
private:
	const units::Game x_, y_, w_, h_; //axis aligned, don't hold rotation.
};

#endif //RECTANGLE_H_