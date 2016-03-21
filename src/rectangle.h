#ifndef RECTANGLE_H_
#define RECTANGLE_H_

struct Rectangle
{
	Rectangle(int x, int y, int width, int height) : x_(x), y_(y), w_(width), h_(height)
	{	}

	int left() const {return x_;}
	int right() const {return x_ + w_;}
	int top() const {return y_;}
	int bottom() const {return y_ + h_;}

	int getWidth() const {return w_;}
	int getHeight() const {return h_;}
private:
	const int x_, y_, w_, h_; //axis aligned, don't hold rotation.
};

#endif //RECTANGLE_H_