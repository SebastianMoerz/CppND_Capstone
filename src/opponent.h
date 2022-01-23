#ifndef OPPONENT_H
#define OPPONENT_H

#include <vector>
#include "SDL.h"
#include "entity.h"


class Opponent : public Entity {
  public:
    Opponent(int x, int y, std::size_t speed, Type type) : Entity(x, y, type), _speed(speed) {}
   
    SDL_Point tryMove();    // emulate brownian motion, currently unused
    void Feed();            // feed instance (increases speed)
    bool isMyTurn();        // check if it's the instance's turn to move

  private:
    int _turnCounter{0};    // game loops since last turn
    int _speed;             // game loops per turn             
};

#endif