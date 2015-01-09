#ifndef LFONT_H
#define LFONT_H

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H

#include "../tex/LSpriteSheet.hpp"

enum LFontTextAlignment {
	LFONT_TEXT_ALIGN_LEFT = 1,
	LFONT_TEXT_ALIGN_CENTERED_H = 2,
	LFONT_TEXT_ALIGN_RIGHT = 4,
	LFONT_TEXT_ALIGN_TOP = 8,
	LFONT_TEXT_ALIGN_CENTERED_V = 16,
	LFONT_TEXT_ALIGN_BOTTOM = 32
};

class LFont : private LSpriteSheet {
	public:
		LFont(void);
		~LFont(void);

		static bool initFreetype(void);

		bool loadFreetype(std::string path, GLuint pixelSize);
		bool loadBitmap(std::string path);
		void freeFont(void);
		void renderText(GLfloat x, GLfloat y, std::string text, LFRect *area = NULL, int align = LFONT_TEXT_ALIGN_LEFT);

		LFRect getStringArea(std::string text);
		GLfloat getLineHeight(void);

	private:
		static FT_Library mLibrary;
		GLfloat mSpace,
						mLineHeight,
						mNewLine;

		GLfloat substringWidth(const char *substring);
		GLfloat stringHeight(const char *thisString);
};

#endif
