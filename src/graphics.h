#ifndef GRAPHICS_H_
#define GRAPHICS_H_

struct SDL_Window;
struct SDL_Rect;
struct SDL_Renderer;
struct SDL_Texture;

struct Graphics
{
	Graphics();
	~Graphics();
	void drawToScreen(SDL_Texture* source, SDL_Rect* source_rectangle, SDL_Rect* destination_rectangle);
private:
	SDL_Window* sdlWindow;
	SDL_Renderer* renderer;
};

#endif //GRAPHICS_H_