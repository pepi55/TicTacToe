#ifndef LTEXTURE_SDL_H
#define LTEXTURE_SDL_H

#include <string>
#include "LSDL.hpp"

class LTextureSDL {
	public:
		LTextureSDL(void);
		~LTextureSDL(void);

		bool loadFromFile(SDL_Renderer *renderer, std::string path);
		bool loadFromRenderedText(SDL_Renderer *renderer, std::string textureText, SDL_Color textColor);

		void free(void);
		void setColor(Uint8 red, Uint8 green, Uint8 blue);
		void setBlendMode(SDL_BlendMode blending);
		void setAlpha(Uint8 alpha);
		void render(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

		int getWidth(void);
		int getHeight(void);

	private:
		SDL_Texture *mTexture;

		int mWidth;
		int mHeight;
};

#endif
