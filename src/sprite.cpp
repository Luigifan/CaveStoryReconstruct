#include "sprite.h"
#include "graphics.h"

#include <SDL2/SDL_image.h>
#include <iostream>

Sprite::Sprite(Graphics& graphics, const std::string& file_path, 
	units::Pixel source_x, units::Pixel source_y, units::Pixel width, units::Pixel height)
{
	sprite_sheet_ = graphics.loadImage(file_path.c_str(), true);
	//SDL_FreeSurface(surf);
	if(sprite_sheet_ == nullptr)
	{
		std::cerr << "Error loading sprite: " << IMG_GetError() << std::endl;
	}
	source_rect_.x = source_x;
	source_rect_.y = source_y;
	source_rect_.w = width;
	source_rect_.h = height;
}

void Sprite::draw(Graphics& graphics, units::Game x, units::Game y)
{
	SDL_Rect destination_rectangle;
	destination_rectangle.x = units::gameToPixel(x);
	destination_rectangle.y = units::gameToPixel(y);
	destination_rectangle.w = source_rect_.w; //don't need w/h but keeping there anyway
	destination_rectangle.h = source_rect_.h;
	graphics.drawToScreen(sprite_sheet_, &source_rect_, &destination_rectangle);
}
