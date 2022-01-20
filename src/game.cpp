#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, std::size_t grid_margin)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  // generate wall around screen    
  for (std::size_t a = grid_margin; a < grid_width-grid_margin; a++) {
      _wall.emplace_back(std::make_shared<Entity>(static_cast<float>(a), static_cast<float>(grid_margin), Entity::Type::kObstacle));                // upper wall
      _wall.emplace_back(std::make_shared<Entity>(static_cast<float>(a), static_cast<float>(grid_height-grid_margin-1), Entity::Type::kObstacle));  // lower wall
  }
  for (std::size_t a = grid_margin; a < grid_height-grid_margin; a++) {
      _wall.emplace_back(std::make_shared<Entity>(static_cast<float>(grid_margin), static_cast<float>(a), Entity::Type::kObstacle));                // left wall
      _wall.emplace_back(std::make_shared<Entity>(static_cast<float>(grid_width-grid_margin-1), static_cast<float>(a), Entity::Type::kObstacle));   // right wall
  }
      
  PlaceFood();
  _opponent = std::make_shared<Entity>(static_cast<float>(-1), static_cast<float>(-1), Entity::Type::kNPC);
  PlaceOpponent();
}


void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food, _wall, _opponent);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  SDL_Point point;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    point = {x,y};
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y) && !this->checkCollision(point)) {
      food.x = x;
      food.y = y;
      return;
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
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y) && !this->checkCollision(point) && food.x != x && food.y != y) {
      _opponent->SetPosition(point);
      return;
    }
  }

}
  
/*
void PlaceEntity(Entity *entity) {
  int x, y;
  SDL_Point point;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    point = {x,y};
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y) && !this->checkCollision(point)) {
      entity->SetPosition(point);
      return;
    }
  }
}
*/

void Game::Update() {
  if (!snake.alive) return;

  // only try to move if direction is not none to avoid unnecessary checkCollision loops.
  if (snake.direction != Snake::Direction::kNone) {
    SDL_Point requestedPosition = snake.tryMove();
    if (this->checkCollisionWithOpponent(requestedPosition)) {
      snake.alive = false;
    }
    else {
      if (!this->checkCollision(requestedPosition)) {snake.Move(requestedPosition);}
    }
  }

  int new_x = static_cast<int>(snake._x);
  int new_y = static_cast<int>(snake._y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    //snake.GrowBody();
    //snake.speed += 0.02;
  }
}
int Game::GetScore() const { return score; }
int Game::GetSize() const { return 0; }

// new
// refactor once snake was replaced with entity of type player
Entity* Game::getBlockingObject(SDL_Point const & point) {
  for (std::shared_ptr<Entity> brick : _wall) {    
    if (((brick->GetPosition()).x == point.x) && ((brick->GetPosition()).y == point.y)) {return brick.get();}    
  }
  return nullptr;
}



bool Game::checkCollision(SDL_Point const & point) {
  for (std::shared_ptr<Entity> brick : _wall) {    
    if (brick->GetPosition().x == point.x && brick->GetPosition().y == point.y) {return true;}    
  }
  return false;
}

bool Game::checkCollisionWithOpponent(SDL_Point const & point) {
  if (_opponent->GetPosition().x == point.x && _opponent->GetPosition().y == point.y) {
    snake.alive = false;
    return true;
  } 
}
  

