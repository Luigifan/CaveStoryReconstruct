#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "sprite.h"

struct Graphics;

struct AnimatedSprite : public Sprite //public because it IS a sprite
{
	AnimatedSprite(Graphics& graphics, const std::string& file_path, 
		units::Pixel source_x, 
		units::Pixel source_y, 
		units::Pixel width, 
		units::Pixel height, 
		units::FPS fps, 
		units::Frame num_frames); //wow

	virtual void update(units::MS elapsed_time_ms) override;
private:
	const units::MS frame_time_; //fps
	const units::Frame num_frames_;
	units::Frame current_frame_;
	units::MS elapsed_time_; //since last frame change
};

#endif
//ANIMATED_SPRITE_H
