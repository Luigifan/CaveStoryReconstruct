#include "graphics.h"

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
    	SDL_WINDOW_SHOWN
    );
    renderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}
Graphics::~Graphics()
{
	SDL_DestroyWindow(sdlWindow);
	SDL_DestroyRenderer(renderer);
}
void Graphics::drawToScreen(SDL_Texture* source, SDL_Rect* source_rectangle, SDL_Rect* destination_rectangle)
{
	SDL_RenderCopy(renderer, source, source_rectangle, destination_rectangle);
}