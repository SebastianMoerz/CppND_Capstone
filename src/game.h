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
  bool checkCollisionWithWall(SDL_Point const &point);
  bool checkCollisionWithOpponent(SDL_Point const & point);
  bool checkCollisionWithFood(SDL_Point const & point, bool erase);
  bool checkCollisionWithPlayer(SDL_Point const & point);
  //Entity* getBlockingObject(SDL_Point const & point);

 private:

  //std::vector<Entity> _allObjects; 
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
  void Update();

  // new
  //void PlaceEntity(Entity& entity);
  void PlaceOpponent();
};

#endif