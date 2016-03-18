#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <string>
struct SDL_Window;
struct SDL_Rect;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Surface;
struct Sprite;
struct AnimatedSprite;

struct Graphics
{
	Graphics();
	~Graphics();
	Sprite* createSprite(const std::string& path, int source_x, int source_y, int width, int height);
	AnimatedSprite* createAnimatedSprite(const std::string& path, int source_x, int source_y, int width, int height, int fps, int frame_count);
	void drawToScreen(SDL_Texture* source, SDL_Rect* source_rectangle, SDL_Rect* destination_rectangle);
	void drawToScreen(SDL_Surface* source, SDL_Rect* source_rectangle, SDL_Rect* destination_rectangle);
	void flip();
	void clear();
private:
	SDL_Window* sdlWindow;
	SDL_Renderer* renderer;
};

#endif //GRAPHICS_H_