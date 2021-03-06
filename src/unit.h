#ifndef UNITS_H_
#define UNITS_H_

#include <cmath>

namespace units {

    typedef unsigned int HP;

    typedef float Game; // Float for extra precision. Intrisic units of position.
    typedef int Pixel; // Integer for discrete units. Pixel values can be +/-.
    typedef unsigned int Tile; // Also discrete, but non-negative.
    typedef unsigned int Frame; // Discrete. Non-negative.

    typedef unsigned int MS; // Discrete Milliseconds. unsigned int matches SDL.
    typedef unsigned int FPS; // Frames per Second (Hz or 1 / Second)

    typedef float Velocity; // Game / MS
    typedef float Acceleration; // Game / MS / MS

    typedef float Degrees;
    typedef float AngularVelocity; //degrees / ms

    namespace {
	    const Game kTileSize = 32.0f;
	    const double kPi = atan(1) * 4; // 22 / 7
    }

    inline Pixel gameToPixel(Game game) {
        // TODO: quit assuming 16x16
        return Pixel(round(game / 2));
    }

    inline Tile gameToTile(Game game) {
        return Tile(game / kTileSize);
    }

    inline Game tileToGame(Tile tile) {
        return tile * kTileSize;
    }

    inline Pixel tileToPixel(Tile tile) {
        return gameToPixel(tileToGame(tile));
    }

    inline double degreesToRadians(Degrees degrees)
    {
	    return degrees * kPi / 180.0f;
    }

    const Game kHalfTile = units::tileToGame(1) / 2.0f;

} // units

#endif // UNITS_H_