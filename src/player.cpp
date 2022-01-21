#include "player.h"
#include <cmath>
#include <iostream>


SDL_Point Player::tryMove() { 
  // temporary position
  int x = _position.x;
  int y = _position.y;
  // calculate requested move
  switch (direction) {
    case Direction::kNone:
      break;

    case Direction::kUp:
      y -= 1;
      break;

    case Direction::kDown:
      y += 1;
      break;

    case Direction::kLeft:
      x -= 1;
      break;

    case Direction::kRight:
      x += 1;
      break;
  }
  // reset direction after moving
  direction = Direction::kNone;

  // Wrap the Player around to the beginning if going off of the screen.
  x = fmod(x + grid_width, grid_width);
  y = fmod(y + grid_height, grid_height);

  SDL_Point requestedPosition{x,y};
  return requestedPosition;
}

// Inefficient method to check if cell is occupied by Player.
bool Player::PlayerCell(int x, int y) {
  if (x == static_cast<int>(_x) && y == static_cast<int>(_y)) {
    return true;
  }
  return false;
}
