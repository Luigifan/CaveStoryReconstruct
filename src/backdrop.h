#ifndef BACKDROP_H_
#define BACKDROP_H_

#include <string>
#include "graphics.h"

//interface
struct Backdrop
{
	virtual void draw(Graphics& graphics) const = 0;
};

///Fixed backdrop. Doesn't move with the player.
struct FixedBackdrop : public Backdrop 
{
	FixedBackdrop(const std::string& path, Graphics& graphics);
	void draw(Graphics& graphics) const override;
private:
	Graphics::TextureID texture_id_;
};

#endif //BACKDROP_H_