#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "player.h"
#include "entity.h"
#include "opponent.h"
#include <memory>

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Player player, std::vector<std::shared_ptr<Entity>> food, std::vector<std::shared_ptr<Entity>> wall, std::vector<std::shared_ptr<Opponent>> opponents);
  void UpdateWindowTitle(int playerScore, int opponentScore, int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif