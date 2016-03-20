#ifndef MAP_H_
#define MAP_H_

#include <vector>
#include <memory>
#include "sprite.h"

struct Graphics;

struct Map
{

  void update(int elapsed_time_ms);
  void draw(Graphics& graphics) const;
private:
  typedef std::vector<std::vector<std::unique_ptr<Sprite>>> ForegroundSprites;
  ForegroundSprites foreground_sprites_;
};

#endif //MAP_H_
