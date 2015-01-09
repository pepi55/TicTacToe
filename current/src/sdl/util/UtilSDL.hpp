#ifndef UTIL_SDL_H
#define UTIL_SDL_H

#include <string>

#include "../../opengl/OpenGL.hpp"
#include "../SDL.hpp"

#include "../../opengl/font/FontProgram2D.hpp"
#include "../../opengl/tex/TexturedPolygon2D.hpp"
#include "../../opengl/font/Font.hpp"

#include "../../player/Player.hpp"

class UtilSDL {
	public:
		UtilSDL(void);
		~UtilSDL(void);

		bool initSDL(void);
		bool initGL(void);
		bool loadMedia(void);

		void loop(void);

		void close(void);

	private:
		void input(void);
		void render(void);

		void reset(void);
		bool mQuit;

		const GLuint SCREEN_WIDTH; //640
		const GLuint SCREEN_HEIGHT; //480
		const GLuint SCREEN_POS_X; //SDL_WINDOWPOS_UNDEFINED
		const GLuint SCREEN_POS_Y; // ``
		const GLchar *const SCREEN_TITLE; //"Generic Title"

		SDL_Window *mWindow;
		SDL_GLContext mContext;

		FontProgram2D mFontProgram;
		Font mFont;
};

#endif
