#include "backdrop.h"
#include "game.h"
#include "unit.h"
#include <SDL2/SDL.h>

namespace {
	const units::Tile kBackgroundSize = 4; //tiles wide
}

FixedBackdrop::FixedBackdrop(const std::string& path, Graphics& graphics)
{
	texture_id_ = graphics.loadImage(path);
}

void FixedBackdrop::draw(Graphics& graphics) const
{
	//It's fixed and doesn't move. Basic right? We just need to fill the screen
	for(units::Tile x = 0; x < Game::kScreenWidth; x += kBackgroundSize)
	{
		for(units::Tile y = 0; y < Game::kScreenHeight; y += kBackgroundSize)
		{
			SDL_Rect destination 
			{ 
				units::tileToPixel(x), 
				units::tileToPixel(y), 
				units::tileToPixel(kBackgroundSize),
				units::tileToPixel(kBackgroundSize)
			};
			graphics.drawToScreen(texture_id_, nullptr, &destination);
		}
	}
}