#ifndef PLAYER_H
#define PLAYER_H

#include "SDL.h"
#include "entity.h"


class Player : public Entity  {
 public:
  Player(int grid_width, int grid_height) : Entity(static_cast<int>(grid_width/2), static_cast<int>(grid_height/2), Type::kPlayer) {}
        
  SDL_Point tryMove();  

  Direction direction = Direction::kNone;
  bool alive{true};

 private: 
};

#endif