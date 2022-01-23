    #include <random>
    #include "SDL.h"
    #include "opponent.h"

    void Opponent::Feed() { if (_speed > 2) {_speed -= 2;}}

    bool Opponent::isMyTurn() 
    {
        ++_turnCounter;    
        if (_turnCounter>_speed) {
            _turnCounter = 0;
            return true;
        }
        return false;
    }

    // not used in current code, can be used to simulate brownian motion instead of directed motion
    SDL_Point Opponent::tryMove() 
    {
      int direction = rand() % 5; // using std::rand() only as POC for moving. of course other engines might be better suited
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