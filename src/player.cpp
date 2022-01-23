#include "player.h"

// calculate new player position based on user input
SDL_Point Player::tryMove() 
{ 
  // temporary position
  int x = this->GetPosition().x;
  int y = this->GetPosition().y;
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

  SDL_Point requestedPosition{x,y};
  return requestedPosition;
}
