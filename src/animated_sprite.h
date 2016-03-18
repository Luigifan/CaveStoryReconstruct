#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "sprite.h"

struct Graphics;
struct SDL_Renderer;

struct AnimatedSprite : public Sprite //public because it IS a sprite
{
	AnimatedSprite(SDL_Renderer* graphics, const std::string& file_path, int source_x, int source_y, int width, int height, int fps, int num_frames); //wow
	virtual void update(int elapsed_time_ms) override;
private:
	const int frame_time_; //fps
	const int num_frames_;
	int current_frame_;
	int elapsed_time_; //since last frame change
};

#endif 
//ANIMATED_SPRITE_H