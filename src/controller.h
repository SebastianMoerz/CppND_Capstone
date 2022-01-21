#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "player.h"

class Controller {
 public:
  void HandleInput(bool &running, Player &player) const;

 private:
  void ChangeDirection(Player &player, Player::Direction input,
                       Player::Direction opposite) const;
};

#endif