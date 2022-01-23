#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <iostream>
#include "SDL.h"
#include <random>


// base class for in-game objects
class Entity {
 public:
 
  enum class Type { kNone, kObstacle, kFood, kPlayer, kNPC}; // rather use inheritance instead of type?
  enum class Direction { kUp, kDown, kLeft, kRight, kNone };
    
  Entity(int x, int y, Type type) : _position({x,y}), _type(type) {}
  
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
   Opponent(int x, int y, Type type) : Entity(x, y, type) { _speed = _startSpeed;}
   
   SDL_Point tryMove() {
      int direction = rand() % 5; // using std::rand() only as POC for moving
      int x = this->GetPosition().x;
      int y = this->GetPosition().y;
      // calculate requested move
      switch (direction) {
        case 0:  //none
          break;

        case 1:  //up
          y -= 1;
          break;

        case 2:  //down
          y += 1;
          break;

        case 3:  //left
          x -= 1;
          break;

        case 4:  //right
          x += 1;
          break;
      }
      
      SDL_Point requestedPosition{x,y};
      return requestedPosition;     
   };

  void Feed() { if (_speed > 1) {--_speed;}}

 bool isMyTurn() {
    ++_turnCounter;    
    if (_turnCounter>_speed) {
      _turnCounter = 0;
      return true;
    }
    return false;
  }  

  private:
    int _turnCounter{0};
    int _speed;
    const int _startSpeed = 15;    
};
 

#endif