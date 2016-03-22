#ifndef MAP_H_
#define MAP_H_

#include <vector>
#include <memory>
#include "sprite.h"
#include "backdrop.h"
#include "unit.h"

struct Graphics;
struct Rectangle;

struct Map
{
	enum TileType
	{
		AIR_TILE, WALL_TILE //solid
	};

	struct CollisionTile{
		CollisionTile(units::Tile row, units::Tile column, TileType type)
		{
			this->row = row;
			this->column = column;
			this->tile_type_ = type;
		}

		TileType tile_type_;
		units::Tile row, column;
	};


	static Map* createTestMap(Graphics& graphics);

	std::vector<CollisionTile> getCollidingTiles(const Rectangle& rectangle) const; //takes the colliding rectangle

  	void update(units::MS elapsed_time_ms);
  	void drawBackground(Graphics& graphics) const;
  	void draw(Graphics& graphics) const;
private:
	struct Tile
	{
		Tile(TileType type = AIR_TILE, std::shared_ptr<Sprite> sprite = std::shared_ptr<Sprite>())
		{
			tile_type_ = type;
			sprite_ = sprite;
		}
		TileType tile_type_;
		std::shared_ptr<Sprite> sprite_;
	};
	std::shared_ptr<Backdrop> backdrop_;
  	typedef std::vector<std::vector<Tile> > ForegroundTiles; //yeet love C++ typedefs tbh
  	typedef std::vector<std::vector<std::shared_ptr<Sprite> > > BackgroundSprites;
  	ForegroundTiles tiles_;
  	BackgroundSprites background_sprites_;

};

#endif //MAP_H_
