#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "player.h"


void Controller::HandleInput(bool &running, Player &player) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          player.direction = Player::Direction::kUp;
          break;

        case SDLK_DOWN:
          player.direction = Player::Direction::kDown;
          break;

        case SDLK_LEFT:
          player.direction = Player::Direction::kLeft;
          break;

        case SDLK_RIGHT:
          player.direction = Player::Direction::kRight;
          break;
      }
    }
  }
}