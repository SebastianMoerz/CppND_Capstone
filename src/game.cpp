#include <algorithm>
#include <iostream>

#include "game.h"
#include "game_utils.h"
#include "SDL.h"


Game::Game(std::size_t grid_width, std::size_t grid_height, std::size_t grid_margin, std::size_t num_food, std::size_t num_opponents, std::size_t init_speed, std::size_t startdist)
    : _grid_max_x(grid_width), _grid_max_y(grid_height), _player(grid_width, grid_height), engine(dev()),
      _NumberOfFoodItems(num_food), _NumberOfOpponents(num_opponents), _InitialOpponentSpeed(init_speed), _MinStartDistance(startdist),
      random_w(grid_margin, static_cast<int>(grid_width - grid_margin - 1)),random_h(grid_margin, static_cast<int>(grid_height - grid_margin - 1)) {

  // generate wall around screen    
  for (std::size_t a = grid_margin; a < grid_width-grid_margin; a++) {  
      _wall.emplace_back(std::make_shared<Entity>(a, grid_margin, Entity::Type::kObstacle));                // upper wall      
      _wall.emplace_back(std::make_shared<Entity>(a, grid_height-grid_margin-1, Entity::Type::kObstacle));  // lower wall
  }
  for (std::size_t a = grid_margin; a < grid_height-grid_margin; a++) {
      _wall.emplace_back(std::make_shared<Entity>(grid_margin, a, Entity::Type::kObstacle));                 // left wall
      _wall.emplace_back(std::make_shared<Entity>(grid_width-grid_margin-1, a, Entity::Type::kObstacle));    // right wall
  }  
  
  PlaceOpponents();
  PlaceFood(); 
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
    renderer.Render(_player, _food, _wall, _opponents);

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
  for (int i = 0; i < _NumberOfFoodItems; i++) {
    while (true) {
      x = random_w(engine);
      y = random_h(engine);
      point = {x,y};      
      if (!DetectCollision(point, &_player) && !DetectCollision(point, _wall) && !DetectCollision(point, _opponents) && !DetectCollision(point, _food)) {     
        _food.emplace_back(std::make_shared<Entity>(x, y, Entity::Type::kFood));  
        break;
      }
    }
  }
}


void Game::PlaceOpponents() {
  int x, y;
  SDL_Point point;
  for (int i = 0; i < _NumberOfOpponents; i++) {
    while (true) {
      x = random_w(engine);
      y = random_h(engine);
      point = {x,y};
      // Check if point is not already occupied and not to close to initial player position
      if (!DetectCollision(point, _wall) && !DetectCollision(point, _opponents) && GameUtils::Heuristic(x, y, _player.GetPosition().x, _player.GetPosition().y) >= _MinStartDistance) {
        _opponents.emplace_back(std::make_shared<Opponent>(x, y, _InitialOpponentSpeed, Entity::Type::kNPC));      
        break;
      }
    }  
  }
}
 
void Game::Update() {
  if (!_player.alive) return;

  // UPDATE PLAYER
  // if user input event in queue, try to move player
  if (_player.direction != Player::Direction::kNone) {
    // calculate the position to which the player wants to move
    SDL_Point requestedPosition = _player.tryMove();
    // check for collisions:
    if (DetectCollision(requestedPosition, _opponents)) {
      // kill player if collision with opponent occured
      _player.alive = false;
    }
    else {
      // update position if movement is not blocked by obstacle
      if (!DetectCollision(requestedPosition, _wall)) {_player.SetPosition(requestedPosition);}
    }
    if (DetectCollision(&_player, _food, true)) {
      // check for collected food
      _playerScore++;
    };
  }

  // UPDATE OPPONENTS
  // only try to move if it's the opponents turn to avoid unnecessary checkCollision loops.
  for (std::shared_ptr<Opponent> opponent : _opponents) {
    if(opponent->isMyTurn()) {
      // find next step toward player position
      SDL_Point requestedPosition = GameUtils::MoveTowardTarget(GetMapOfObstacles(), opponent->GetPosition(), _player.GetPosition());
      
      if (DetectCollision(requestedPosition, &_player)) {
        // kill player if collision with opponent occured
        _player.alive = false;
      }
      else {
        // update position if movement is not blocked by obstacle (wall or other opponent)
        if (!DetectCollision(requestedPosition, _wall) && !DetectCollision(requestedPosition, _opponents)) {opponent->SetPosition(requestedPosition);}
      }
      if (DetectCollision(opponent.get(), _food, true)) {
        // check for collected food and increase opponent's speed
        _opponentScore++;
        opponent->Feed();
      };
    }
  }

  // "Game Over" message
  if (!_player.alive) { 
    std::cout << "GAME OVER: You were eaten!" << std::endl;
  }
  if (_food.empty()) { 
    _won = true;
    std::cout << "YOU WON!" << std::endl;
  }
}


// Collision Detection #1: Collision between Point and Entity (used to check if movement to Point is valid)
bool Game::DetectCollision(SDL_Point point, Entity *entity) {
  if (entity != nullptr) {
    return (point.x == entity->GetPosition().x && point.y == entity->GetPosition().y);
  }
  return false;
}

// Collision Detection #2: Collision between two Entities
bool Game::DetectCollision(Entity *first, Entity *second) {
  if (first != nullptr && second != nullptr) {
    return (first->GetPosition().x == second->GetPosition().x && first->GetPosition().y == second->GetPosition().y);
  }
  return false;
}

// Collision Detection #3: Collision between Entity and Vector of Entities (via shared ptr). Used e.g. for wall collision detection
bool Game::DetectCollision(Entity *first, std::vector<std::shared_ptr<Entity>> &second) {
  if (first != nullptr && !second.empty()) {
    auto it = std::find_if(second.begin(), second.end(), [&first](const std::shared_ptr<Entity>& item){return first->GetPosition().x == item->GetPosition().x && first->GetPosition().y == item->GetPosition().y;});
    if (it != second.end()) { return true; }      
  }  
  return false;
}

// Collision Detection #4: Collision between Point and Vector of Entities (via shared ptr). Used to check if movement to Point is valid e.g. for movement or for initial placement of objects
bool Game::DetectCollision(SDL_Point point, std::vector<std::shared_ptr<Entity>> &entities) {
  if (!entities.empty()) {
    auto it = std::find_if(entities.begin(), entities.end(), [&point](const std::shared_ptr<Entity>& item){return point.x == item->GetPosition().x && point.y == item->GetPosition().y;});
    if (it != entities.end()) { return true; }  
  }  
  return false;
}

// Collision Detection #5: Collision between Point and Vector of Opponents (via shared ptr). Used to check if movement to Point is valid e.g. for movement or for initial placement of objects
bool Game::DetectCollision(SDL_Point point, std::vector<std::shared_ptr<Opponent>> &opponents) {
  if (!opponents.empty()) {
    auto it = std::find_if(opponents.begin(), opponents.end(), [&point](const std::shared_ptr<Opponent>& item){return point.x == item->GetPosition().x && point.y == item->GetPosition().y;});
    if (it != opponents.end()) { return true; }  
  }  
  return false;
}

// Collision Detection #6: Collision between Entity and Vector of Entities (via shared ptr). If collision occurs, remove the object which caused the collision. Used for collision with food items.
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

// get a representation of the game map where each element is either "kNone" or "kObstacle"
// used as input for calculating the opponent's movement
std::vector<std::vector<Entity::Type>> Game::GetMapOfObstacles () {
  std::vector<std::vector<Entity::Type>> grid {};
  std::vector<Entity::Type> row {};
  
  // init emtpy grid (of format grid[y][x])
  for (int y = 0; y < _grid_max_y; y++) {
    row.clear();
    for (int x = 0; x < _grid_max_x; x++) {
      row.push_back(Entity::Type::kNone);
    }
    grid.push_back(row);
  }  

  // add obstacles to grid - in this case only the wall blocks
  for (std::shared_ptr<Entity> brick : _wall) {
    grid[brick->GetPosition().y][brick->GetPosition().x] = Entity::Type::kObstacle;    
  }  
  return grid;
}