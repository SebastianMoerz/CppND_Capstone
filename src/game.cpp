#include "game.h"
#include <iostream>
#include "SDL.h"
#include <algorithm>
#include <game_utils.h>

Game::Game(std::size_t grid_width, std::size_t grid_height, std::size_t grid_margin)
    : _grid_max_x(grid_width), _grid_max_y(grid_height), _player(grid_width, grid_height), engine(dev()), 
      random_w(0, static_cast<int>(grid_width - 1)),random_h(0, static_cast<int>(grid_height - 1)) {

  // generate wall around screen    
  for (std::size_t a = grid_margin; a < grid_width-grid_margin; a++) {  
      _wall.emplace_back(std::make_shared<Entity>(a, grid_margin, Entity::Type::kObstacle));                // upper wall
      _wall.emplace_back(std::make_shared<Entity>(a, grid_height-grid_margin-1, Entity::Type::kObstacle));  // lower wall
  }
  for (std::size_t a = grid_margin; a < grid_height-grid_margin; a++) {
      _wall.emplace_back(std::make_shared<Entity>(grid_margin, a, Entity::Type::kObstacle));                 // left wall
      _wall.emplace_back(std::make_shared<Entity>(grid_width-grid_margin-1, a, Entity::Type::kObstacle));    // right wall
  }  
  PlaceFood();  
  _opponent = std::make_shared<Opponent>(-1, -1, Entity::Type::kNPC);
  PlaceOpponent();
}


void Game::Run(Controller const &controller, Renderer &renderer, std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, _paused, _player);
    if (!_paused && !_won && _player.alive) {
      Update();
    }
    renderer.Render(_player, _food, _wall, _opponent);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(_playerScore, _opponentScore, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is smaller than the target ms_per_frame), delay the loop to achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;  
  SDL_Point point;
  for (int i = 0; i < 40; i++) {
    while (true) {
      x = random_w(engine);
      y = random_h(engine);
      point = {x,y};      
      if (!this->DetectCollision(point, &_player) && !this->DetectCollision(point, _wall) && !this->DetectCollision(point, _food)) {     
        _food.emplace_back(std::make_shared<Entity>(x, y, Entity::Type::kFood));  
        break;
      }
    }
  }
}

void Game::PlaceOpponent() {
  int x, y;
  SDL_Point point;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    point = {x,y};
    // Check that the location is not occupied by a Player item before placing food.
    if (!this->DetectCollision(point, &_player) && !this->DetectCollision(point, _wall) && !this->DetectCollision(point, _food)) {
      _opponent->SetPosition(point);
      return;
    }
  }

}
 
void Game::Update() {
  if (!_player.alive) return;

  // only try to move if direction is not none to avoid unnecessary checkCollision loops.
  if (_player.direction != Player::Direction::kNone) {
    SDL_Point requestedPosition = _player.tryMove();
    if (this->DetectCollision(requestedPosition, _opponent.get())) {
      _player.alive = false;
    }
    else {
      if (!this->DetectCollision(requestedPosition, _wall)) {_player.SetPosition(requestedPosition);}
    }
    if (this->DetectCollision(&_player, _food, true)) {
      _playerScore++;
    };
  }

  // only try to move if it's the opponents turn to avoid unnecessary checkCollision loops.
  if(_opponent->isMyTurn()) {
    //SDL_Point requestedPosition = _opponent->tryMove();
    SDL_Point requestedPosition = GameUtils::MoveTowardTarget(GetMapOfObstacles(), _opponent->GetPosition(), _player.GetPosition());
    
    if (this->DetectCollision(requestedPosition, &_player)) {
      _player.alive = false;
    }
    else {
      if (!this->DetectCollision(requestedPosition, _wall)) {_opponent->SetPosition(requestedPosition);}
    }
    if (this->DetectCollision(_opponent.get(), _food, true)) {
      _opponentScore++;
      _opponent->Feed();
    };
  }

  if (!_player.alive) { 
    std::cout << "GAME OVER: You were eaten!" << std::endl;
  }
  if (_food.empty()) { 
    _won = true;
    std::cout << "YOU WON!" << std::endl;
  }
}

int Game::GetPlayerScore() const { return _playerScore; }
int Game::GetOpponentScore() const { return _opponentScore; }
int Game::GetSize() const { return 0; }


bool Game::DetectCollision(SDL_Point point, Entity *entity) {
  if (entity != nullptr) {
    return (point.x == entity->GetPosition().x && point.y == entity->GetPosition().y);
  }
  return false;
}

bool Game::DetectCollision(Entity *first, Entity *second) {
  if (first != nullptr && second != nullptr) {
    return (first->GetPosition().x == second->GetPosition().x && first->GetPosition().y == second->GetPosition().y);
  }
  return false;
}

bool Game::DetectCollision(Entity *first, std::vector<std::shared_ptr<Entity>> &second) {
  if (first != nullptr && !second.empty()) {
    auto it = std::find_if(second.begin(), second.end(), [&first](const std::shared_ptr<Entity>& item){return first->GetPosition().x == item->GetPosition().x && first->GetPosition().y == item->GetPosition().y;});
    if (it != second.end()) { return true; }  
  }  
  return false;
}

bool Game::DetectCollision(SDL_Point point, std::vector<std::shared_ptr<Entity>> &entities) {
  if (!entities.empty()) {
    auto it = std::find_if(entities.begin(), entities.end(), [&point](const std::shared_ptr<Entity>& item){return point.x == item->GetPosition().x && point.y == item->GetPosition().y;});
    if (it != entities.end()) { return true; }  
  }  
  return false;
}

bool Game::DetectCollision(Entity *first, std::vector<std::shared_ptr<Entity>> &second, bool erase) {
  if (first != nullptr && !second.empty()) {
    auto it = std::find_if(second.begin(), second.end(), [&first](const std::shared_ptr<Entity>& item){return first->GetPosition().x == item->GetPosition().x && first->GetPosition().y == item->GetPosition().y;});
    if (it != second.end()) {       
      if (erase) {second.erase(it);}
      return true;
    }  
  }  
  return false;
}

std::vector<std::vector<Entity::Type>> Game::GetMapOfObstacles () {
  std::vector<std::vector<Entity::Type>> grid {};
  std::vector<Entity::Type> row {};
  
  // init emtpy grid (of for,at grid[y][x])
  for (int y = 0; y < _grid_max_y; y++) {
    row.clear();
    for (int x = 0; x < _grid_max_x; x++) {
      row.push_back(Entity::Type::kNone);
    }
    grid.push_back(row);
  }  

  // add obstacles to grid - in this case only the wall blocks
  for (std::shared_ptr<Entity> brick : this->_wall) {
    grid[brick->GetPosition().y][brick->GetPosition().x] = Entity::Type::kObstacle;    
  }  
  return grid;
}