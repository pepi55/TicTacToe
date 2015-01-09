#ifndef LUTIL_SDL_H
#define LUTIL_SDL_H

#include <string>
#include <sstream>

#include "../sdl/LSDL.hpp"
#include "../sdl/LTextureSDL.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_POS_X = SDL_WINDOWPOS_UNDEFINED;
const int SCREEN_POS_Y = SDL_WINDOWPOS_UNDEFINED;
const char *const SCREEN_TITLE = "Generic Title";

bool initSDL(void);
bool loadMedia(void);
void mainLoop(void);
void render(void);
void closeSDL(void);

#endif
