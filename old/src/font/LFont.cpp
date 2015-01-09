#include "LFont.hpp"

FT_Library LFont::mLibrary;

LFont::LFont(void) {
	mSpace = 0.0f;
	mLineHeight = 0.0f;
	mNewLine = 0.0f;
}

LFont::~LFont(void) {
	freeFont();
}

bool LFont::initFreetype(void) {
#ifndef __FREEGLUT_H__
	FT_Error error = FT_Init_Freetype(&mLibrary);
	if (error) {
		fprintf(stderr, "Freetype error:%X\n", error);
		return false;
	}
#endif

	return true;
}

bool LFont::loadFreetype(std::string path, GLuint pixelSize) {
	FT_Error error = 0x0;

#ifdef __FREEGLUT_H__
	error = FT_Init_FreeType(&mLibrary);
	if (error) {
		fprintf(stderr, "Freetype error:%X\n", error);
		return false;
	}
#endif

	GLuint cellW = 0;
	GLuint cellH = 0;
	int maxBearing = 0;
	int minHang = 0;

	LTexture bitmaps[256];
	FT_Glyph_Metrics metrics[256];

	FT_Face face = NULL;
	error = FT_New_Face(mLibrary, path.c_str(), 0, &face);
	if (!error) {
		error = FT_Set_Pixel_Sizes(face, 0, pixelSize);
		if (!error) {
			for (int i = 0; i < 256; ++i) {
				error = FT_Load_Char(face, i, FT_LOAD_RENDER);
				if (!error) {
					metrics[i] = face->glyph->metrics;
					bitmaps[i].copyPixels8(face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows);

					if (metrics[i].horiBearingY / 64 > maxBearing) {
						maxBearing = metrics[i].horiBearingY / 64;
					}

					if (metrics[i].width / 64 > (int)cellW) {
						cellW = metrics[i].width / 64;
					}

					int glyphHang = (metrics[i].horiBearingY - metrics[i].height) / 64;
					if (glyphHang < minHang) {
						minHang = glyphHang;
					}
				} else {
					fprintf(stderr, "Unable to load glyph. Freetype error:%X\n", error);
					error = 0x0;
				}
			}

			cellH = maxBearing - minHang;
			createPixels8(cellW * 16, cellH * 16);

			GLuint currentChar = 0;
			LFRect nextClip = {0.0f, 0.0f, (GLfloat)cellW, (GLfloat)cellH};

			int bX = 0,
					bY = 0;

			for (unsigned int rows = 0; rows < 16; rows++) {
				for (unsigned int cols = 0; cols < 16; cols++) {
					bX = cellW * cols; bY = cellH * rows;

					nextClip.x = bX;
					nextClip.y = bY;
					nextClip.w = metrics[currentChar].width / 64;
					nextClip.h = cellH;

					bitmaps[currentChar].blitPixels8(bX, bY + maxBearing - metrics[currentChar].horiBearingY / 64, *this);
					mClips.push_back(nextClip);
					currentChar++;
				}
			}

			padPixels8();

			if (loadTextureFromPixels8()) {
				if (!generateDataBuffer(LSPRITE_ORIGIN_TOP_LEFT)) {
					fprintf(stderr, "Unable to create vertex buffer for bitmap font!\n");
					error = 0xA2;
				}
			} else {
				fprintf(stderr, "Unable to create texture from generated bitmap font!\n");
				error = 0xA2;
			}

			glBindTexture(GL_TEXTURE_2D, getTextureID());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

			mSpace = cellW / 2;
			mNewLine = maxBearing;
			mLineHeight = cellH;
		} else {
			fprintf(stderr, "Unable to set font size! Freetype error:%X\n", error);
		}

		FT_Done_Face(face);
	} else {
		fprintf(stderr, "Unable to load font face! Freetype error:%X\n", error);
		return false;
	}

#ifdef __FREEGLUT_H__
	FT_Done_FreeType(mLibrary);
#endif

	return error == 0x0;
}

bool LFont::loadBitmap(std::string path) {
	bool success = true;
	const GLuint BLACK_PIXEL = 0x00;

	freeFont();

	if (loadPixelsFromFile8(path)) {
		GLfloat cellW = imageWidth() / 16.0f;
		GLfloat cellH = imageHeight() / 16.0f;

		GLuint top = cellH;
		GLuint bottom = 0;
		GLuint aBottom = 0;

		int pX = 0;
		int pY = 0;
		int bX = 0;
		int bY = 0;

		GLuint currentChar = 0;
		LFRect nextClip = {0.0f, 0.0f, cellH, cellW};

		for (unsigned int rows = 0; rows < 16; ++rows) {
			for (unsigned int cols = 0; cols < 16; ++cols) {
				bX = cellW * cols;
				bY = cellH * rows;

				nextClip.x = cellW * cols;
				nextClip.y = cellH * rows;
				nextClip.w = cellW;
				nextClip.h = cellH;

				//Left side
				for (int pCol = 0; pCol < cellW; ++pCol) {
					for (int pRow = 0; pRow < cellH; ++pRow) {
						pX = bX + pCol;
						pY = bY + pRow;

						if (getPixel8(pX, pY) != BLACK_PIXEL) {
							nextClip.x = pX;

							pCol = cellW;
							pRow = cellH;
						}
					}
				}

				//Right side
				for (int pCol_w = cellW - 1; pCol_w >= 0; --pCol_w) {
					for (int pRow_w = 0; pRow_w < cellH; ++pRow_w) {
						pX = bX + pCol_w;
						pY = bY + pRow_w;

						if (getPixel8(pX, pY) != BLACK_PIXEL) {
							nextClip.w = (pX - nextClip.x) + 1;

							pCol_w = -1;
							pRow_w = cellH;
						}
					}
				}

				//Top side
				for (int pRow = 0; pRow < cellH; ++pRow) {
					for (int pCol = 0; pCol < cellW; ++pCol) {
						pX = bX + pCol;
						pY = bY + pRow;

						if (getPixel8(pX, pY) != BLACK_PIXEL) {
							if (pRow < (int)top) {
								top = pRow;
							}

							pCol = cellW;
							pRow = cellH;
						}
					}
				}

				//Bottom side
				for (int pRow_b = cellH - 1; pRow_b >= 0; --pRow_b) {
					for (int pCol_b = 0; pCol_b < cellW; ++pCol_b) {
						pX = bX + pCol_b;
						pY = bY + pRow_b;

						if (getPixel8(pX, pY) != BLACK_PIXEL) {
							if (currentChar == 'A') {
								aBottom = pRow_b;
							}

							if (pRow_b > (int)bottom) {
								bottom = pRow_b;
							}

							pCol_b = cellW;
							pRow_b = -1;
						}
					}
				}

				mClips.push_back(nextClip);
				currentChar++;
			}
		}

		for (int t = 0; t < 256; ++t) {
			mClips[t].y += top;
			mClips[t].h -= top;
		}

		if (loadTextureFromPixels8()) {
			if (!generateDataBuffer(LSPRITE_ORIGIN_TOP_LEFT)) {
				fprintf(stderr, "Unable to create vertex buffer for bitmap font!\n");
				success = false;
			}
		} else {
			fprintf(stderr, "Unable to create texture from bitmap font pixels!\n");
			success = false;
		}

		glBindTexture(GL_TEXTURE_2D, getTextureID());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		mSpace = cellW / 2;
		mNewLine = aBottom - top;
		mLineHeight = bottom - top;
	} else {
		fprintf(stderr, "Could not load bitmap font image %s!\n", path.c_str());
		success = false;
	}

	return success;
}

void LFont::freeFont(void) {
	freeTexture();

	mSpace = 0.0f;
	mNewLine = 0.0f;
	mLineHeight = 0.0f;
}

void LFont::renderText(GLfloat x, GLfloat y, std::string text, LFRect *area, int align) {
	if (getTextureID() != 0) {
		GLfloat dX = x;
		GLfloat dY = y;

		if (area != NULL) {
			if (align == 0) {
				align = LFONT_TEXT_ALIGN_LEFT | LFONT_TEXT_ALIGN_TOP;
			}

			if (align & LFONT_TEXT_ALIGN_LEFT) {
				dX = area->x;
			} else if (align & LFONT_TEXT_ALIGN_CENTERED_H) {
				dX = area->x + (area->w - substringWidth(text.c_str())) / 2.0f;
			} else if (align & LFONT_TEXT_ALIGN_RIGHT) {
				dX = area->x + (area->w - substringWidth(text.c_str()));
			}
			
			if (align & LFONT_TEXT_ALIGN_TOP) {
				dY = area->y;
			} else if (align & LFONT_TEXT_ALIGN_CENTERED_V) {
				dY = area->y + (area->h - stringHeight(text.c_str())) / 2.0f;
			} else if (align & LFONT_TEXT_ALIGN_BOTTOM) {
				dY = area->y + (area->h - stringHeight(text.c_str()));
			}
		}

		glTranslatef(dX, dY, 0.0f);
		glBindTexture(GL_TEXTURE_2D, getTextureID());

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mVertexDataBuffer);
			glTexCoordPointer(2, GL_FLOAT, sizeof(LVertexData2D), (GLvoid*)offsetof(LVertexData2D, texCoord));
			glVertexPointer(2, GL_FLOAT, sizeof(LVertexData2D), (GLvoid*)offsetof(LVertexData2D, position));

			for (int i = 0; i < (int)text.length(); ++i) {
				if (text[i] == ' ') {
					glTranslatef(mSpace, 0.0f, 0.0f);
					dX += mSpace;
				} else if (text[i] == '\n') {
					GLfloat targetX = x;

					if (area != NULL) {
						if (align & LFONT_TEXT_ALIGN_LEFT) {
							targetX = area->x;
						} else if (align & LFONT_TEXT_ALIGN_CENTERED_H) {
							targetX = area->x + (area->w - substringWidth(&text.c_str()[i + 1])) / 2.0f;
						} else if (align & LFONT_TEXT_ALIGN_RIGHT) {
							targetX = area->x + (area->w - substringWidth(&text.c_str()[i + 1]));
						}
					}

					glTranslatef(targetX - dX, mNewLine, 0.0f);
					dY += mNewLine;
					dX += targetX - dX;
				} else {
					GLuint ascii = (unsigned char)text[i];

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffers[ascii]);
					glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, NULL);

					glTranslatef(mClips[ascii].w, 0.0f, 0.0f);
					dX += mClips[ascii].w;
				}
			}
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

GLfloat LFont::stringHeight(const char *thisString) {
	GLfloat height = mLineHeight;

	for (int i = 0; thisString[i] != '\0'; ++i) {
		if (thisString[i] == '\n') {
			height += mLineHeight;
		}
	}

	return height;
}

GLfloat LFont::substringWidth(const char *substring) {
	GLfloat subWidth = 0.0f;

	for (int i = 0; (i < substring[i]) != '\0' && substring[i] != '\n'; ++i) {
		if (substring[i] == ' ') {
			subWidth += mSpace;
		} else {
			GLuint ascii = (unsigned char)substring[i];
			subWidth += mClips[ascii].w;
		}
	}

	return subWidth;
}

LFRect LFont::getStringArea(std::string text) {
	GLfloat subWidth = 0.0f;
	LFRect area = {0.0f, 0.0f, subWidth, mLineHeight};

	for (int i = 0; i < (int)text.length(); ++i) {
		if (text[i] == ' ') {
			subWidth += mSpace;
		} else if (text[i] == '\n') {
			area.h += mLineHeight;

			if (subWidth > area.w) {
				area.w = subWidth;
				subWidth = 0.0f;
			}
		} else {
			GLuint ascii = (unsigned char)text[i];
			subWidth += mClips[ascii].w;
		}
	}

	if (subWidth > area.w) {
		area.w = subWidth;
	}

	return area;
}

GLfloat LFont::getLineHeight(void) {
	return mLineHeight;
}
