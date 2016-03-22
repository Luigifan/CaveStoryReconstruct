#include "backdrop.h"
#include "game.h"
#include <SDL2/SDL.h>

namespace {
	const int kBackgroundSize = 128; //pixels
}

FixedBackdrop::FixedBackdrop(const std::string& path, Graphics& graphics)
{
	texture_id_ = graphics.loadImage(path);
}

void FixedBackdrop::draw(Graphics& graphics) const
{
	//It's fixed and doesn't move. Basic right? We just need to fill the screen
	
	for(int x = 0; x < Game::kScreenWidth; x += kBackgroundSize)
	{
		for(int y = 0; y < Game::kScreenHeight; y += kBackgroundSize)
		{
			SDL_Rect destination {x, y, kBackgroundSize, kBackgroundSize};
			graphics.drawToScreen(texture_id_, NULL, &destination);
		}
	}
}