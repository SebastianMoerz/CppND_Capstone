#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

//new
#include <vector>
#include "entity.h"
#include <memory>

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, std::size_t grid_margin);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

  // new
  bool checkCollision(SDL_Point const &point);
  bool checkCollisionWithOpponent(SDL_Point const & point);
  Entity* getBlockingObject(SDL_Point const & point);

 private:

  //std::vector<Entity> _allObjects; 
  std::vector<std::shared_ptr<Entity>> _wall; 
  std::vector<std::shared_ptr<Entity>> _coins;
  std::shared_ptr<Entity> _opponent;

  Snake snake;
  SDL_Point food;
  

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();  
  void Update();

  // new
  //void PlaceEntity(Entity& entity);
  void PlaceOpponent();
};

#endif