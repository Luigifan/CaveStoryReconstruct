#include "game.h"

#include <stdlib.h>
#include <algorithm>
#include <SDL2/SDL.h>
#include "graphics.h"
#include "player.h"
#include "input.h"
#include "map.h"
#include "first_cave_bat.h"

namespace {
  const units::FPS kTargetFramesPerSecond = 60;
  const units::MS kMaxFrameTime = 5 * 1000 / kTargetFramesPerSecond;
}

units::Tile Game::kScreenWidth = 20 ; //640
units::Tile Game::kScreenHeight = 15 ; //480
units::Pixel Game::kWindowWidth = units::tileToPixel(kScreenWidth * 2);
units::Pixel Game::kWindowHeight = units::tileToPixel(kScreenHeight * 2);

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

void Game::update(units::MS elapsed_time_ms)
{
	//map_->update(elapsed_time_ms);

	player_->update(elapsed_time_ms, *map_); //dereference that bitch
	bat_->update(elapsed_time_ms, player_->getCenterX());
	//printf("Checking collisions\n");
	if(bat_->damageRectangle().collidesWith(player_->getDamageRect()))
		printf("\tDo damage!\n");
}

void Game::draw(Graphics& graphics)
{
  graphics.clear();
  map_->drawBackground(graphics);
  player_->draw(graphics);
  bat_->draw(graphics);
  map_->draw(graphics);
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
  graphics.setWindowText("Cave Story");
  Input input;
  player_.reset(new Player(graphics, units::tileToGame(kScreenWidth / 2), units::tileToGame(kScreenHeight / 2)));
  bat_.reset(new FirstCaveBat(graphics, units::tileToGame(7), units::tileToGame(kScreenHeight / 2 + 1)));
  map_.reset(Map::createTestMap(graphics));

  units::MS last_update_time = SDL_GetTicks();
  while(running)
  {
    const units::MS startTimeMs = SDL_GetTicks();
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
    // Walking logic
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

    //looking logic, probably going to split this off into a seperate method later because this is getting messy
    if((input.isKeyHeld(SDLK_UP)) && (input.isKeyHeld(SDLK_DOWN)))
    {
      player_->lookHorizontal();
    }
    else if(input.isKeyHeld(SDLK_UP))
    {
      player_->lookUp();
    }
    else if(input.isKeyHeld(SDLK_DOWN))
    {
      player_->lookDown();
    }
    else
    {
      player_->lookHorizontal();
    }

    // player jump logic
    if(input.wasKeyPressed(SDLK_z))
    {
      player_->startJump();
    }
    else if(input.wasKeyReleased(SDLK_z))
    {
      player_->stopJump();
    }


    const units::MS current_time_ms = SDL_GetTicks();
    const units::MS elapsed_time_between_updates = current_time_ms - last_update_time;
    //this is what ensures frame times are consistent
    update(std::min(elapsed_time_between_updates, kMaxFrameTime));
    last_update_time = current_time_ms;

    draw(graphics);

    const units::MS ms_per_frame = 1000 / kTargetFramesPerSecond;
    const units::MS elapsed_time_ms = SDL_GetTicks() - startTimeMs;

    //graphics.setWindowText("Cave Story - " + std::to_string(int(1.0f / (ms_per_frame - elapsed_time_ms) * 1000.0f)) + "fps");
    if(elapsed_time_ms < ms_per_frame)
    {
      SDL_Delay(ms_per_frame - elapsed_time_ms);
    }

    /*const int endTimeMs = SDL_GetTicks();
    const int elapsedTime = endTimeMs - startTimeMs; //ms it took
    const int delayTimeToMaintainFPS = 1000 / kTargetFramesPerSecond;
    SDL_Delay(delayTimeToMaintainFPS - elapsedTime);*/
    //printf("fps: %f\n", 1.0f / (delayTimeToMaintainFPS / 1000.0f));
  }
  SDL_Quit();
}
