#ifndef ENTITY_H
#define ENTITY_H

#include "SDL.h"

// base class for in-game objects (no separate .cpp file due to trivial member definition)
class Entity {
 public:
 
  enum class Type { kNone, kObstacle, kFood, kPlayer, kNPC}; // rather use inheritance instead of type?
  enum class Direction { kUp, kDown, kLeft, kRight, kNone };
    
  Entity(int x, int y, Type type) : _position({x,y}), _type(type) {}
  
  void SetPosition(SDL_Point const &position) {_position = position;}
  SDL_Point GetPosition() {return _position;}

  // not used yet
  // void SetType(Type const &type) {_type = type;}
  // Type GetType() {return _type;}

private:
  Type _type;
  SDL_Point _position{0,0}; 
};

#endif