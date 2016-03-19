#ifndef SPRITE_H_
#define SPRITE_H_

#include <string>
#include <SDL2/SDL.h>

struct Graphics;

struct Sprite
{
	//Sprite(const std::string& file_path, int source_x, int source_y, int width, int height);
	Sprite(Graphics& graphics, const std::string& file_path, int source_x, int source_y, int width, int height);

	virtual void update(int /*elapsed time in ms*/){} //leave empty for later mostly for classes that extend sprite
	void draw(Graphics& graphics, int x, int y);
protected:
	SDL_Rect source_rect_; //protected so we can modify in inherited classes later
private:
	SDL_Texture* sprite_sheet_;
};

#endif
//SPRITE_H_
