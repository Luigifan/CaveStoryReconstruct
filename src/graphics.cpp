#include "graphics.h"
#include "sprite.h"

#include <iostream>
#include <SDL2/SDL.h>

namespace {
	const int kScreenWidth = 640;
  	const int kScreenHeight = 480;
  	const int kBitsPerPixel = 32;
}

Graphics::Graphics()
{
	sdlWindow = SDL_CreateWindow("yeet",
    	SDL_WINDOWPOS_UNDEFINED,
    	SDL_WINDOWPOS_UNDEFINED,
    	kScreenWidth,
    	kScreenHeight,
    	SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    renderer = SDL_CreateRenderer(sdlWindow, -1, 0);
    SDL_RenderSetLogicalSize(renderer, kScreenWidth, kScreenHeight);
}
Graphics::~Graphics()
{
	SDL_DestroyWindow(sdlWindow);
	SDL_DestroyRenderer(renderer);
}

Sprite* Graphics::createSprite(const std::string& path, int source_x, int source_y, int width, int height)
{
	return new Sprite(renderer, path, source_x, source_y, width, height);
}

void Graphics::clear()
{
	SDL_RenderClear(renderer);
}
void Graphics::flip()
{
	SDL_RenderPresent(renderer);
}
void Graphics::drawToScreen(SDL_Texture* source, SDL_Rect* source_rectangle, SDL_Rect* destination_rectangle)
{
	if(SDL_RenderCopy(renderer, source, source_rectangle, destination_rectangle) != 0)
	{
		std::cerr << "Error: " << SDL_GetError() << std::endl;
	}
}
void Graphics::drawToScreen(SDL_Surface* source, SDL_Rect* source_rectangle, SDL_Rect* destination_rectangle)
{
	SDL_Texture* converted = SDL_CreateTextureFromSurface(renderer, source);
	if(converted == nullptr)
	{
		std::cerr << "Error: " << SDL_GetError() << std::endl;
	}
	int res = SDL_RenderCopy(renderer, converted, source_rectangle, destination_rectangle);
	if(res != 0)
	{
		std::cerr << "Error: " << SDL_GetError() << std::endl;
	}
	SDL_DestroyTexture(converted);
}