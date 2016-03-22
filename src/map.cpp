#include "map.h"
#include "graphics.h"
#include "game.h"
#include "rectangle.h"

using std::vector;
using std::unique_ptr;
using std::shared_ptr;

//static 
Map* Map::createTestMap(Graphics& graphics)
{
  const shared_ptr<Sprite> EmptySprite(new Sprite(graphics, "content/Stage/PrtCave.pbm", 0, 0, 0, 0));


  Map* map = new Map();

  map->backdrop_.reset(new FixedBackdrop("content/bkBlue.pbm", graphics));

  const units::Tile num_rows = 15; //15 * 32 = 480
  const units::Tile num_cols = 20; //20 * 32 = 640 screen w/h

  //ensure tiles is the proper size
  map->tiles_ = vector<vector<Tile > >(num_rows, vector<Tile >( num_cols, Tile() ) );
  map->background_sprites_ = vector<vector<shared_ptr<Sprite> > >(num_rows, vector<shared_ptr<Sprite>>(num_cols, nullptr));
  //map->background_sprites_ = vector<vector<shared_ptr<Sprite> > >(num_rows, shared_ptr<Sprite>( num_cols, EmptySprite ) );

  //filler tile
  Tile tile(WALL_TILE, std::shared_ptr<Sprite>(new Sprite(graphics, "content/Stage/PrtCave.pbm", units::tileToPixel(1), 0, units::tileToPixel(1), units::tileToPixel(1))));

  const units::Tile row_solid = 11;
  for(units::Tile col = 0; col < num_cols; col++)
  {
    //map->foreground_sprites_[row_solid][col] = std::move(sprite);
    map->tiles_[row_solid][col] = tile;
  }
  map->tiles_[10][5] = tile;
  map->tiles_[9][4] = tile;
  map->tiles_[8][3] = tile;

  shared_ptr<Sprite> chain_top(new Sprite(graphics, "content/Stage/PrtCave.pbm", units::tileToPixel(11), units::tileToPixel(2), units::tileToPixel(1), units::tileToPixel(1)));
  shared_ptr<Sprite> chain_middle(new Sprite(graphics, "content/Stage/PrtCave.pbm", units::tileToPixel(12), units::tileToPixel(2), units::tileToPixel(1), units::tileToPixel(1)));
  shared_ptr<Sprite> chain_bottom(new Sprite(graphics, "content/Stage/PrtCave.pbm", units::tileToPixel(13), units::tileToPixel(2), units::tileToPixel(1), units::tileToPixel(1)));

  map->background_sprites_[9][3] = chain_top;
  map->background_sprites_[10][3] = chain_bottom;

  return map;
}

std::vector<Map::CollisionTile> Map::getCollidingTiles(const Rectangle& rectangle) const
{
  const units::Tile first_row = units::gameToTile(rectangle.top());
  const units::Tile last_row = units::gameToTile(rectangle.bottom());
  const units::Tile first_column = units::gameToTile(rectangle.left());
  const units::Tile last_column = units::gameToTile(rectangle.right());
  //printf("top to bottom: %d; %d; %d; %d\n", first_row, last_row, first_column, last_column);

  // 1. Calculate first and last row for collision based on our rectangle's position
  std::vector<Map::CollisionTile> collision_tiles;

  // 2. Iterate inclusively creating a collision tile deciding collision based on tile type at row, column
  for(units::Tile row = first_row; row <= last_row; row++)
  {
    for(units::Tile column = first_column; column <= last_column; column++)
    {
      collision_tiles.push_back(CollisionTile(row, column, tiles_[row][column].tile_type_)); //yay
    }
  }

  return collision_tiles;
}

void Map::drawBackground(Graphics& graphics) const
{
  backdrop_->draw(graphics);

  for(size_t row = 0; row < background_sprites_.size(); row++)
  {
    for(size_t column = 0; column < background_sprites_[row].size(); column++)
    {
      if(background_sprites_[row][column])
      {
        background_sprites_[row][column]->draw(graphics, units::tileToGame(column), units::tileToGame(row));
      }
    }
  }
}

void Map::draw(Graphics& graphics) const
{
  //rectangular grid
  for(size_t row = 0; row < tiles_.size(); row++)
  {
    for(size_t column = 0; column < tiles_[row].size(); column++)
    {
      if(tiles_[row][column].sprite_)
      {
        tiles_[row][column].sprite_->draw(graphics, units::tileToGame(column), units::tileToGame(row));

      }
    }
  }
}

void Map::update(units::MS elapsed_time_ms)
{
  //rectangular grid
  for(size_t row = 0; row < tiles_.size(); row++)
  {
    for(size_t column = 0; column < tiles_[row].size(); column++)
    {
      if(tiles_[row][column].sprite_)
      {
        tiles_[row][column].sprite_->update(elapsed_time_ms);
      }
    }
  }
}
