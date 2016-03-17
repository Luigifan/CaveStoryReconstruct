#include "game.h"

#include <stdlib.h>
#include "graphics.h"
#include <SDL2/SDL.h> //i hope

namespace { //idk what this is
  const int kTargetFramesPerSecond = 60;
}

Game::Game()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  //SDL_ShowCursor(SDL_DISABLE);
}

Game::~Game()
{
  SDL_Quit();
}

void Game::runEventLoop()
{
  this->eventLoop();
}

void Game::update()
{}

void Game::draw()
{}

void Game::eventLoop()
{
  printf("Running game with target frame rate of %d\n\n", kTargetFramesPerSecond);

  //Here is where we'll handle events, timers (and their callbacks >.>)
  //Also we'll handle update and draw (in that order)
  //Also, I'll be using SDL2 from the start as I have a little bit of experience with it already
  bool running = true;
  SDL_Event event;
  Graphics graphics; //when this loop exits, this will be deconstructed
  while(running)
  {
    const int startTimeMs = SDL_GetTicks();
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_KEYDOWN:
          if(event.key.keysym.sym == SDLK_ESCAPE)
          {running = false;} //breaks out of our event loop
          break;
        default:
            break;
      }
    }
    update();
    draw();

    const int endTimeMs = SDL_GetTicks();
    const int elapsedTime = endTimeMs - startTimeMs; //ms it took
    const int delayTimeToMaintainFPS = 1000 / kTargetFramesPerSecond;
    SDL_Delay(delayTimeToMaintainFPS);
    //printf("fps: %f\n", 1.0f / (delayTimeToMaintainFPS / 1000.0f));
  }
}
