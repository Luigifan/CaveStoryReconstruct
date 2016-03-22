#ifndef SPRITE_H_
#define SPRITE_H_

#include <string>
#include <SDL2/SDL.h>
#include "unit.h"

struct Graphics;

struct Sprite
{
	//Sprite(const std::string& file_path, int source_x, int source_y, int width, int height);
	Sprite(Graphics& graphics, 
		const std::string& file_path, 
		units::Pixel source_x, 
		units::Pixel source_y, 
		units::Pixel width, 
		units::Pixel height); //We use Pixel here because it directly translates to our bitmap file

	virtual void update(units::MS /*elapsed time in ms*/){} //leave empty for later mostly for classes that extend sprite
	void draw(Graphics& graphics, units::Game x, units::Game y); //Do conversions inside draw method. We keep our precision till the very end
protected:
	SDL_Rect source_rect_; //protected so we can modify in inherited classes later
private:
	SDL_Texture* sprite_sheet_;
};

#endif
//SPRITE_H_
