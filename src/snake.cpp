#include "snake.h"
#include <cmath>
#include <iostream>


void Snake::Move(SDL_Point newPos) {
  _x = static_cast<float>(newPos.x);
  _y = static_cast<float>(newPos.y);
}

SDL_Point Snake::tryMove() { 
  // temporary position
  float x = _x;
  float y = _y;
  // calculate requested move
  switch (direction) {
    case Direction::kNone:
      break;

    case Direction::kUp:
      y -= 1.0;
      break;

    case Direction::kDown:
      y += 1.0;
      break;

    case Direction::kLeft:
      x -= 1.0;
      break;

    case Direction::kRight:
      x += 1.0;
      break;
  }
  // reset direction after moving
  direction = Direction::kNone;


  // Wrap the Snake around to the beginning if going off of the screen.
  x = fmod(x + grid_width, grid_width);
  y = fmod(y + grid_height, grid_height);

  SDL_Point requestedPosition{static_cast<int>(x),static_cast<int>(y)};
  return requestedPosition;
}

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(_x) && y == static_cast<int>(_y)) {
    return true;
  }
  return false;
}