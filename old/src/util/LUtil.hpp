#ifndef LUTIL_H
#define LUTIL_H

#include "../ogl/LOpenGL.hpp"

#include "../tex/LTexture.hpp"
#include "../tex/LSpriteSheet.hpp"
#include "../tex/LTexturedPolygon2D.hpp"

#include "../prim/LPlainPolygonProgram2D.hpp"
#include "../prim/LMultiColorPolygonProgram2D.hpp"
#include "../prim/LMultiPolygonProgram2D.hpp"

#include "../font/LFont.hpp"
#include "../vertex/LMultiColorVertex2D.hpp"
#include "../vertex/LVertexPos2D.hpp"
#include "../shader/LShaderProgram.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;
const int SCREEN_POSITION[] = {200, 60};
const char *const SCREEN_TITLE = "Generic Title";

bool initGL(void);
bool loadGP(void);
bool loadMedia(void);

void update(void);
void render(void);
void renderScene(void);
void handleKeys(unsigned char key, int x, int y);
void handleMouseMotion(int x, int y);

#endif
