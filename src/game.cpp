#include "game.h"

#include <stdlib.h>
#include <SDL2/SDL.h>
#include "graphics.h"
#include "player.h"
#include "input.h"

namespace { //idk what this is
  const int kTargetFramesPerSecond = 60;
}
int Game::kTileSize = 32;

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

void Game::update(int elapsed_time_ms)
{
  player_->update(elapsed_time_ms);
}

void Game::draw(Graphics& graphics)
{
  graphics.clear();
  player_->draw(graphics);
  graphics.flip();
}

void Game::eventLoop()
{
  printf("Running game with target frame rate of %d\n\n", kTargetFramesPerSecond);

  //Here is where we'll handle events, timers (and their callbacks >.>)
  //Also we'll handle update and draw (in that order)
  //Also, I'll be using SDL2 from the start as I have a little bit of experience with it already
  bool running = true;
  SDL_Event event;
  Graphics graphics; //when this loop exits, this will be deconstructed
  Input input;
  player_.reset(new Player(graphics, 320, 240));

  int last_update_time = SDL_GetTicks();
  while(running)
  {
    const int startTimeMs = SDL_GetTicks();
    input.beginNewFrame();
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_KEYDOWN:
          input.keyDownEvent(event);
          break;
        case SDL_KEYUP:
          input.keyUpEvent(event);
          break;
        default:
            break;
      }
    }

    if(input.wasKeyPressed(SDLK_ESCAPE))
    {running = false;}
    // if both left and right pressed, stop moving
    // left = move left
    // right = move right
    // else, stop moving all together

    if((input.isKeyHeld(SDLK_LEFT)) && (input.isKeyHeld(SDLK_RIGHT)))
    {
      player_->stopMoving();
    }
    else if(input.isKeyHeld(SDLK_LEFT))
    {
      player_->startMovingLeft();
    }
    else if(input.isKeyHeld(SDLK_RIGHT))
    {
      player_->startMovingRight();
    }
    else
      player_->stopMoving();


    const int current_time_ms = SDL_GetTicks();
    update(current_time_ms - last_update_time);
    last_update_time = current_time_ms;

    draw(graphics);

    const int endTimeMs = SDL_GetTicks();
    const int elapsedTime = endTimeMs - startTimeMs; //ms it took
    const int delayTimeToMaintainFPS = 1000 / kTargetFramesPerSecond;
    SDL_Delay(delayTimeToMaintainFPS);
    //printf("fps: %f\n", 1.0f / (delayTimeToMaintainFPS / 1000.0f));
  }
  SDL_Quit();
}
