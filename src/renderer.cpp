#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Darwin - Eat, don't be eaten!", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Player player,  std::vector<std::shared_ptr<Entity>> food, std::vector<std::shared_ptr<Entity>> wall, std::shared_ptr<Opponent> opponent) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  if (!food.empty()) {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
    for (std::shared_ptr<Entity> fooditem : food) {
      block.x = fooditem->GetPosition().x * block.w;
      block.y = fooditem->GetPosition().y * block.h;
      SDL_RenderFillRect(sdl_renderer, &block);
    }
  }

  // Render wall
  if (!wall.empty()) {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (std::shared_ptr<Entity> brick : wall) {      
      block.x = brick->GetPosition().x * block.w;
      block.y = brick->GetPosition().y * block.h;
      SDL_RenderFillRect(sdl_renderer, &block);
    }
  }

  // Render opponent
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  block.x = opponent->GetPosition().x * block.w;
  block.y = opponent->GetPosition().y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render player
  block.x = player.GetPosition().x * block.w;
  block.y = player.GetPosition().y * block.h;
  if (player.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int playerScore, int opponentScore, int fps) {
  std::string title{"DARWIN - Eat or be eaten || Player's Score: " + std::to_string(playerScore) + ", Opponents's Score: " + std::to_string(opponentScore) + " || FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
