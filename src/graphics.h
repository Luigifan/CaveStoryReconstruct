#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <string>
#include <map>
struct SDL_Window;
struct SDL_Rect;
struct SDL_Renderer;
struct SDL_Texture;
struct Sprite;
struct AnimatedSprite;

struct Graphics
{
	typedef SDL_Texture* TextureID;
	Graphics();
	~Graphics();
	TextureID loadImage(const std::string& file_path);
	//Sprite* createSprite(const std::string& path, int source_x, int source_y, int width, int height);
	//AnimatedSprite* createAnimatedSprite(const std::string& path, int source_x, int source_y, int width, int height, int fps, int frame_count);
	void drawToScreen(TextureID source, SDL_Rect* source_rectangle, SDL_Rect* destination_rectangle);
	void flip();
	void clear();
private:
	typedef std::map<std::string, SDL_Texture*> SpriteMap;
	SpriteMap sprite_sheets_;
	SDL_Window* sdlWindow;
	SDL_Renderer* renderer;
};

#endif //GRAPHICS_H_
