#include "game.h"

#include <SDL2/SDL.h> //i hope

namespace { //idk what this is
  const int kScreenWidth = 640;
  const int kScreenHeight = 480;
  const int kBitsPerPixel = 32;
  const int kTargetFramesPerSecond = 60;
}

Game::Game()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_ShowCursor(SDL_DISABLE);
  sdlScreen = SDL_CreateWindow("yeet",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    kScreenWidth,
    kScreenHeight,
    SDL_WINDOW_SHOWN);
}

Game::~Game()
{
  SDL_DestroyWindow(sdlScreen);
  SDL_Quit();
}

void Game::runEventLoop()
{
  this->eventLoop();
}

void Game::update()
{

}

void Game::draw()
{

}

void Game::eventLoop()
{
  //Here is where we'll handle events, timers (and their callbacks >.>)
  //Also we'll handle update and draw (in that order)
  //Also, I'll be using SDL2 from the start as I have a little bit of experience with it already
  bool running = true;
  SDL_Event event;
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

    //This is for compensation
    const int elapsedTime = SDL_GetTicks() - startTimeMs;
    //Ensures our loop only happens 60 times per second.
    // 1 / 60 seconds
    // 1000 / 60 ms
    //SDL_Delay takes ms
    SDL_Delay(1000 / kTargetFramesPerSecond - elapsedTime);
  }
}
