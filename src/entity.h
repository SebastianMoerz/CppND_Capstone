#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <iostream>
#include "SDL.h"
#include <random>


// base class for in-game objects
class Entity {
 public:
 
  enum class Type { kObstacle, kFood, kPlayer, kNPC}; // rather use inheritance instead of type?
  enum class Direction { kUp, kDown, kLeft, kRight, kNone };
  
  Entity(){}
  Entity(int x, int y, Type type) : _position({x,y}), _type(type) {}
  ~Entity(){}

  void SetPosition(SDL_Point const &position) {_position = position;}
  SDL_Point GetPosition() {return _position;}

  void SetType(Type const &type) {_type = type;}
  Type GetType() {return _type;}

private:
  Type _type;
  SDL_Point _position{0,0}; 
};

class Opponent : public Entity {
  public:
   Opponent(int x, int y, Type type, std::mt19937 &engine) : Entity(x, y, type), engine(&engine) {}
   SDL_Point tryMove() {
     //std::cout << "tryMove: ";
     //int dir = randomWalk(*engine);
     int dir = rand() % 5;
     //std::cout << dir << std::endl;
       // temporary position
      int x = this->GetPosition().x;
      int y = this->GetPosition().y;
      // calculate requested move
      switch (dir) {
        case 0:
          break;

        case 1:
          y -= 1;
          break;

        case 2:
          y += 1;
          break;

        case 3:
          x -= 1;
          break;

        case 4:
          x += 1;
          break;
      }
      // reset direction after moving      

      // Wrap the Player around to the beginning if going off of the screen.
      //x = fmod(x + grid_width, grid_width);
      //y = fmod(y + grid_height, grid_height);

      SDL_Point requestedPosition{x,y};
      return requestedPosition;     
   };
  
  bool isMyTurn() {
    ++_turnCounter;    
    if (_turnCounter>speed) {
      _turnCounter = 0;
      return true;
    }
    return false;
  }

  private:
    std::mt19937 *engine;    //currently unused
    std::uniform_int_distribution<int> randomWalk{(0,4)}; // currently unused
    int _turnCounter{0};
    const int speed = 3;    
};

// unused
/*
public:
  enum class Direction { kUp, kDown, kLeft, kRight, kNone }; // only for movable entities?
  Entity(Type type) : _type(type) {};
  Entity(float x, float y, Type type) : _x(x), _y(y), _type(type) {}; 
  ~Entity();
  SDL_Point GetPosition() {
      SDL_Point position{static_cast<int>(_x),static_cast<int>(_y)};
      return position;
  };
  void SetPosition(SDL_Point positon) {
      _x = static_cast<float>(positon.x);
      _y = static_cast<float>(positon.y);
  }  
private:
  //float _x{0}; // stationary entities have SDL_Points, only movable objects have float
  //float _y{0};
*/

#endif