#include "map.h"
#include "graphics.h"
#include "game.h"

using std::vector;
using std::unique_ptr;
using std::shared_ptr;

//static 
Map* Map::createTestMap(Graphics& graphics)
{
  Map* map = new Map();

  const int num_rows = 15; //15 * 32 = 480
  const int num_cols = 20; //20 * 32 = 640 screen w/h

  //ensure foreground sprites is the proper size
  map->foreground_sprites_ = vector<vector<shared_ptr<Sprite> > >(num_rows, vector<shared_ptr<Sprite> >( num_cols, nullptr ) );

  //filler tile
  std::shared_ptr<Sprite> sprite(new Sprite(graphics, "content/PrtCave.bmp", Game::kTileSize, 0, Game::kTileSize, Game::kTileSize));

  const int row_solid = 11;
  for(int col = 0; col < num_cols; col++)
  {
    //map->foreground_sprites_[row_solid][col] = std::move(sprite);
    map->foreground_sprites_[row_solid][col] = sprite;
  }
  map->foreground_sprites_[10][5] = sprite;
  map->foreground_sprites_[9][4] = sprite;
  map->foreground_sprites_[8][3] = sprite;
  map->foreground_sprites_[7][2] = sprite;

  map->foreground_sprites_[10][5] = sprite;

  return map;
}

void Map::draw(Graphics& graphics) const
{
  //rectangular grid
  for(size_t row = 0; row < foreground_sprites_.size(); row++)
  {
    for(size_t column = 0; column < foreground_sprites_[row].size(); column++)
    {
      if(foreground_sprites_[row][column])
      {
        foreground_sprites_[row][column]->draw(graphics, column*Game::kTileSize, row*Game::kTileSize);
      }
    }
  }
}

void Map::update(int elapsed_time_ms)
{
  //rectangular grid
  for(size_t row = 0; row < foreground_sprites_.size(); row++)
  {
    for(size_t column = 0; column < foreground_sprites_[row].size(); column++)
    {
      if(foreground_sprites_[row][column])
      {
        foreground_sprites_[row][column]->update(elapsed_time_ms);
      }
    }
  }
}
