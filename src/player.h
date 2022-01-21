#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "SDL.h"
#include "entity.h"


class Player  {
 public:

  enum class Direction { kUp, kDown, kLeft, kRight, kNone };

  Player(int grid_width, int grid_height) : grid_width(grid_width), grid_height(grid_height),_position({static_cast<int>(grid_width/2),static_cast<int>(grid_height/2)}) {}
        
  bool PlayerCell(int x, int y);

  Direction direction = Direction::kNone;

  bool alive{true};
  float _x;
  float _y;

  void SetPosition(SDL_Point const &position) {_position = position;}
  SDL_Point GetPosition() {return _position;}

  //new
  SDL_Point tryMove();  

 private:  
 
  SDL_Point _position;
  int grid_width;
  int grid_height;

};

#endif