#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "player.h"

//new
#include <vector>
#include "entity.h"
#include <memory>

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, std::size_t grid_margin);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetPlayerScore() const;
  int GetOpponentScore() const;
  int GetSize() const;

  // new
  bool DetectCollision(SDL_Point point, Entity *entity);
  bool DetectCollision(Entity *first, Entity *second);
  bool DetectCollision(Entity *first, std::vector<std::shared_ptr<Entity>> &second);
  bool DetectCollision(SDL_Point point, std::vector<std::shared_ptr<Entity>> &entities);
  bool DetectCollision(Entity *first, std::vector<std::shared_ptr<Entity>> &second, bool erase);

 private:

  std::vector<std::shared_ptr<Entity>> _wall; 
  std::vector<std::shared_ptr<Entity>> _food;
  std::shared_ptr<Opponent> _opponent;

  Player _player;  

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int _playerScore{0};
  int _opponentScore{0};

  void PlaceFood(); 
  void PlaceOpponent(); 
  void Update();  
};

#endif