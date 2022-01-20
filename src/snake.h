#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"


class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight, kNone };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        _x(grid_width / 2),
        _y(grid_height / 2) {}

  //void Update();

  //void GrowBody();
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kNone;

  //float speed{0.1f};
  //int size{1};
  bool alive{true};
  float _x;
  float _y;
  // std::vector<SDL_Point> body;

  //new
  void Move(SDL_Point newPos);
  SDL_Point tryMove();

 private:
  
  //void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
  // Game* _game; // handle to game
};

#endif