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
  const shared_ptr<Sprite> EmptySprite(new Sprite(graphics, "content/PrtCave.bmp", 0, 0, 0, 0));


  Map* map = new Map();

  map->backdrop_.reset(new FixedBackdrop("content/bkBlue.bmp", graphics));

  const int num_rows = 15; //15 * 32 = 480
  const int num_cols = 20; //20 * 32 = 640 screen w/h

  //ensure tiles is the proper size
  map->tiles_ = vector<vector<Tile > >(num_rows, vector<Tile >( num_cols, Tile() ) );
  map->background_sprites_ = vector<vector<shared_ptr<Sprite> > >(num_rows, vector<shared_ptr<Sprite>>(num_cols, nullptr));
  //map->background_sprites_ = vector<vector<shared_ptr<Sprite> > >(num_rows, shared_ptr<Sprite>( num_cols, EmptySprite ) );

  //filler tile
  Tile tile(WALL_TILE, std::shared_ptr<Sprite>(new Sprite(graphics, "content/PrtCave.bmp", Game::kTileSize, 0, Game::kTileSize, Game::kTileSize)));

  const int row_solid = 11;
  for(int col = 0; col < num_cols; col++)
  {
    //map->foreground_sprites_[row_solid][col] = std::move(sprite);
    map->tiles_[row_solid][col] = tile;
  }
  map->tiles_[10][5] = tile;
  map->tiles_[9][4] = tile;
  map->tiles_[8][3] = tile;

  shared_ptr<Sprite> chain_top(new Sprite(graphics, "content/PrtCave.bmp", 11*Game::kTileSize, 2*Game::kTileSize, Game::kTileSize, Game::kTileSize));
  shared_ptr<Sprite> chain_middle(new Sprite(graphics, "content/PrtCave.bmp", 12*Game::kTileSize, 2*Game::kTileSize, Game::kTileSize, Game::kTileSize));
  shared_ptr<Sprite> chain_bottom(new Sprite(graphics, "content/PrtCave.bmp", 13*Game::kTileSize, 2*Game::kTileSize, Game::kTileSize, Game::kTileSize));

  map->background_sprites_[9][3] = chain_top;
  map->background_sprites_[10][3] = chain_bottom;

  return map;
}

std::vector<Map::CollisionTile> Map::getCollidingTiles(const Rectangle& rectangle) const
{
  const int first_row = rectangle.top() / Game::kTileSize;
  const int last_row = rectangle.bottom() / Game::kTileSize;
  const int first_column = rectangle.left() / Game::kTileSize;
  const int last_column = rectangle.right() / Game::kTileSize;

  // 1. Calculate first and last row for collision based on our rectangle's position
  std::vector<Map::CollisionTile> collision_tiles;

  // 2. Iterate inclusively creating a collision tile deciding collision based on tile type at row, column
  for(int row = first_row; row <= last_row; row++)
  {
    for(int column = first_column; column <= last_column; column++)
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
        background_sprites_[row][column]->draw(graphics, column*Game::kTileSize, row*Game::kTileSize);
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
        tiles_[row][column].sprite_->draw(graphics, column*Game::kTileSize, row*Game::kTileSize);

      }
    }
  }
}

void Map::update(int elapsed_time_ms)
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
