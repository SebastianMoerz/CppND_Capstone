#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <iostream>
#include "SDL.h"


// base class for in-game objects
class Entity {
 public:
 
  enum class Type { kObstacle, kTreasure, kPlayer, kNPC}; // rather use inheritance instead of type?
  enum class Direction { kUp, kDown, kLeft, kRight, kNone }; // only for movable entities?
  
  Entity();
  //Entity(Type type) : _type(type) {};
  Entity(float x, float y, Type type) : _x(x), _y(y), _type(type) {}; 
  //~Entity();



  SDL_Point GetPosition() {
      SDL_Point position{static_cast<int>(_x),static_cast<int>(_y)};
      return position;
  };
  void SetPosition(SDL_Point positon) {
      _x = static_cast<float>(positon.x);
      _y = static_cast<float>(positon.y);
  }  

private:
  Type _type;
  float _x{0}; // entities have SDL_Points, only movable objects have float
  float _y{0};
};

#endif