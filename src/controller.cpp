#include <iostream>
#include <string>

#include "controller.h"
#include "SDL.h"
#include "player.h"

void Controller::HandleInput(bool &running, bool &paused,  Player &player) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_p: { 
          std::string s = paused == false ? "Pause" : "Resume";
          std::cout << s << " game" << std::endl;
          paused = paused == false ? true : false;
          }
          break;
        case SDLK_UP:
          if (!paused) { player.direction = Player::Direction::kUp; };
          break;

        case SDLK_DOWN:
          if (!paused) { player.direction = Player::Direction::kDown; };
          break;

        case SDLK_LEFT:
          if (!paused) { player.direction = Player::Direction::kLeft; };
          break;

        case SDLK_RIGHT:
         if (!paused) { player.direction = Player::Direction::kRight; };
          break;
      }
    }
  }
}