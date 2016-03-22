#include "graphics.h"
#include "sprite.h"
#include "animated_sprite.h"
#include "game.h"

#include <iostream>
#include <cassert>
#include <SDL2/SDL.h>

namespace {
  	const int kBitsPerPixel = 32;
}

Graphics::Graphics()
{
	sdlWindow = SDL_CreateWindow(NULL,
    	SDL_WINDOWPOS_UNDEFINED,
    	SDL_WINDOWPOS_UNDEFINED,
    	units::tileToPixel(Game::kScreenWidth),
    	units::tileToPixel(Game::kScreenHeight),
    	SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    renderer = SDL_CreateRenderer(sdlWindow, -1, 0);
    SDL_RenderSetLogicalSize(renderer, units::tileToPixel(Game::kScreenWidth), units::tileToPixel(Game::kScreenHeight));
}
Graphics::~Graphics()
{
	for(SpriteMap::iterator iter = sprite_sheets_.begin(); iter != sprite_sheets_.end(); iter++)
	{
		//it's a pair
		//first would be the string (name)
		SDL_DestroyTexture(iter->second); //second is the value
	}
	SDL_DestroyWindow(sdlWindow);
	SDL_DestroyRenderer(renderer);

}

void Graphics::setWindowText(const std::string& newWindowTitle) const
{
  SDL_SetWindowTitle(sdlWindow, newWindowTitle.c_str());
}

Graphics::TextureID Graphics::loadImage(const std::string& file_path, bool blackTransparent)
{
	if(sprite_sheets_.count(file_path) == 0) //doesn't exist, need to actually load
	{
		//here we load
		SDL_Surface* surf = SDL_LoadBMP(file_path.c_str());
		if(blackTransparent)
			SDL_SetColorKey(surf, SDL_TRUE, 0); //0 is black

    assert(surf != nullptr);
    sprite_sheets_[file_path] = SDL_CreateTextureFromSurface(renderer, surf);
		if(surf != nullptr)
			SDL_FreeSurface(surf);
	}

	return sprite_sheets_[file_path];
}

void Graphics::clear()
{
	SDL_RenderClear(renderer);
}
void Graphics::flip()
{
	SDL_RenderPresent(renderer);
}

void Graphics::drawToScreen(TextureID source, SDL_Rect* source_rectangle, SDL_Rect* destination_rectangle)
{
	if(SDL_RenderCopy(renderer, source, source_rectangle, destination_rectangle) != 0)
	{
		std::cerr << "Error: " << SDL_GetError() << std::endl;
	}
}
