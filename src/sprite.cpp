#include "sprite.h"
#include "graphics.h"

#include <SDL2/SDL_image.h>
#include <iostream>

/*Sprite::Sprite(const std::string& file_path, int source_x, int source_y, int width, int height)
{
	sprite_sheet_ = SDL_LoadBMP(file_path.c_str());
	std::cout << sprite_sheet_ << std::endl;
	if(sprite_sheet_ == nullptr)
	{
		std::cerr << "Error loading sprite: " << SDL_GetError() << std::endl;
	}
	source_rect_.x = source_x;
	source_rect_.y = source_y;
	source_rect_.w = width;
	source_rect_.h = height;
}*/

Sprite::Sprite(SDL_Renderer* graphics, const std::string& file_path, int source_x, int source_y, int width, int height)
{
	//SDL_Surface* surf = SDL_LoadBMP(file_path.c_str());
	//sprite_sheet_ = SDL_CreateTextureFromSurface(graphics, surf);
	sprite_sheet_ = IMG_LoadTexture(graphics, file_path.c_str());
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

Sprite::~Sprite()
{
	SDL_DestroyTexture(sprite_sheet_);
}

void Sprite::draw(Graphics& graphics, int x, int y)
{
	SDL_Rect destination_rectangle;
	destination_rectangle.x = x;
	destination_rectangle.y = y;
	destination_rectangle.w = source_rect_.w;
	destination_rectangle.h = source_rect_.h;
	graphics.drawToScreen(sprite_sheet_, &source_rect_, &destination_rectangle);
}