#ifndef FONT_H
#define FONT_H

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H

#include "../tex/SpriteSheet.hpp"
#include "FontProgram2D.hpp"

enum FontTextAlignment {
	FONT_TEXT_ALIGN_LEFT = 1,
	FONT_TEXT_ALIGN_CENTERED_H = 2,
	FONT_TEXT_ALIGN_RIGHT = 4,
	FONT_TEXT_ALIGN_TOP = 8,
	FONT_TEXT_ALIGN_CENTERED_V = 16,
	FONT_TEXT_ALIGN_BOTTOM = 32
};

class Font : private SpriteSheet {
	public:
		Font(void);
		~Font(void);

		static bool initFreetype(void);

		bool loadFreetype(std::string path, GLuint pixelSize);
		bool loadBitmap(std::string path);
		void freeFont(void);
		void renderText(GLfloat x, GLfloat y, std::string text, FRect *area = NULL, int align = FONT_TEXT_ALIGN_LEFT);

		static void setFontProgram2D(FontProgram2D *fontProgram);

		FRect getStringArea(std::string text);
		GLfloat getLineHeight(void);

	private:
		static FontProgram2D *mFontProgram2D;
		static FT_Library mLibrary;
		GLfloat mSpace,
						mLineHeight,
						mNewLine;

		GLfloat substringWidth(const char *substring);
		GLfloat stringHeight(const char *thisString);
};

#endif
