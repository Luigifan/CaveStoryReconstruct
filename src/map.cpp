#include "map.h"
#include "graphics.h"
#include "game.h"

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
