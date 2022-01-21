#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "SDL.h"
#include "entity.h"


class Player : public Entity  {
 public:

  enum class Direction { kUp, kDown, kLeft, kRight, kNone };

  Player(int grid_width, int grid_height) : Entity(static_cast<int>(grid_width/2), static_cast<int>(grid_height/2), Type::kPlayer), grid_width(grid_width), grid_height(grid_height) {}
        
  bool PlayerCell(int x, int y);

  Direction direction = Direction::kNone;

  bool alive{true};

  //new
  SDL_Point tryMove();  

 private: 
  int grid_width;
  int grid_height;

};

#endif