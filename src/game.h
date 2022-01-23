#ifndef GAME_H
#define GAME_H

#include <random>
#include <vector>
#include <memory>

#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "player.h"
#include "opponent.h"
#include "entity.h"


class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, std::size_t grid_margin, std::size_t food, std::size_t opponents, std::size_t speed, std::size_t startdist);

  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetPlayerScore() const { return _playerScore; };
  int GetOpponentScore() const { return _opponentScore; };
  int GetSize() const { return 0; };

  // overloaded function for collision detection. returns true if collision, else false
  bool DetectCollision(SDL_Point point, Entity *entity);
  bool DetectCollision(Entity *first, Entity *second);
  bool DetectCollision(Entity *first, std::vector<std::shared_ptr<Entity>> &second);
  bool DetectCollision(SDL_Point point, std::vector<std::shared_ptr<Entity>> &entities);
  bool DetectCollision(SDL_Point point, std::vector<std::shared_ptr<Opponent>> &opponents);
  bool DetectCollision(Entity *first, std::vector<std::shared_ptr<Entity>> &second, bool erase);

  // get a representation of the game map where each element is either "kNone" or "kObstacle"
  std::vector<std::vector<Entity::Type>> GetMapOfObstacles();

 private:

  // vectors for objects with graphic representation
  std::vector<std::shared_ptr<Entity>> _wall;
  std::vector<std::shared_ptr<Entity>> _food;
  std::vector<std::shared_ptr<Opponent>> _opponents;

  // no pointer, since number of players is always one
  Player _player;  

  std::size_t _NumberOfFoodItems; 
  std::size_t _NumberOfOpponents; 
  std::size_t _InitialOpponentSpeed;
  std::size_t _MinStartDistance;
  std::size_t _grid_max_x;
  std::size_t _grid_max_y;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int _playerScore{0};
  int _opponentScore{0};

  bool _paused{false};
  bool _won{false};

  void PlaceFood(); 
  void PlaceOpponents(); 
  void Update();    
};

#endif