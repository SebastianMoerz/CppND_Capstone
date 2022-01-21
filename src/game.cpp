#include "game.h"
#include <iostream>
#include "SDL.h"
#include <algorithm>

Game::Game(std::size_t grid_width, std::size_t grid_height, std::size_t grid_margin)
    : _player(grid_width, grid_height), engine(dev()), random_w(0, static_cast<int>(grid_width - 1)),random_h(0, static_cast<int>(grid_height - 1)) {

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
  _opponent = std::make_shared<Opponent>(-1, -1, Entity::Type::kNPC, engine);
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
    controller.HandleInput(running, _player);
    Update();
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
      if (!_player.PlayerCell(x, y) && !this->checkCollisionWithWall(point) && !this->checkCollisionWithFood(point, false)) {     
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
    if (!_player.PlayerCell(x, y) && !this->checkCollisionWithWall(point) && !this->checkCollisionWithFood(point, false)) {
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
    if (this->checkCollisionWithOpponent(requestedPosition)) {
      _player.alive = false;
    }
    else {
      if (!this->checkCollisionWithWall(requestedPosition)) {_player.SetPosition(requestedPosition);}
    }
    if (this->checkCollisionWithFood(_player.GetPosition(), true)) {
      _playerScore++;
    };
  }


  // only try to move if it's the opponents turn to avoid unnecessary checkCollision loops.
  if (_opponent->isMyTurn()) {
    SDL_Point requestedPosition = _opponent->tryMove();
    if (this->checkCollisionWithPlayer(requestedPosition)) {
      _player.alive = false;
    }
    else {
      if (!this->checkCollisionWithWall(requestedPosition)) {_opponent->SetPosition(requestedPosition);}
    }
    if (this->checkCollisionWithFood(_opponent->GetPosition(), true)) {
      _opponentScore++;
    };
  }
}

  //int new_x = static_cast<int>(player._x);
  // int new_y = static_cast<int>(player._y);
 

int Game::GetPlayerScore() const { return _playerScore; }
int Game::GetOpponentScore() const { return _opponentScore; }
int Game::GetSize() const { return 0; }



bool Game::checkCollisionWithWall(SDL_Point const & point) {
  if (!_wall.empty()) {
    for (std::shared_ptr<Entity> brick : _wall) {    
      if (brick->GetPosition().x == point.x && brick->GetPosition().y == point.y) {return true;}    
    }
  }
  return false;
}

bool Game::checkCollisionWithOpponent(SDL_Point const & point) {
  if (_opponent->GetPosition().x == point.x && _opponent->GetPosition().y == point.y) {
    //_player.alive = false;
    return true;
  }
  return false; 
}

bool Game::checkCollisionWithPlayer(SDL_Point const & point) {
  if (_player.GetPosition().x == point.x && _player.GetPosition().y == point.y) {
    //_player.alive = false;
    return true;
  } 
  return false;
}


bool Game::checkCollisionWithFood(SDL_Point const & point, bool erase) {    
  if (!_food.empty()) { 
    auto it = std::find_if(_food.begin(), _food.end(), [&point](const std::shared_ptr<Entity>& f){return point.x == f->GetPosition().x && point.y == f->GetPosition().y;});
    if (it != _food.end()) {       
      if (erase) {_food.erase(it);}
      return true;
    }  
  }  
  return false;
}

