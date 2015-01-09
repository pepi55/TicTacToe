#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include "TexturedPolygon2D.hpp"
#include "data/Rect.hpp"
#include "../OpenGL.hpp"

class Texture {
	public:
		Texture(void);
		virtual ~Texture(void);

		static void setTexturedPolygon2D(TexturedPolygon2D *program);

		bool lock(void);
		bool unlock(void);

		void render(GLfloat x, GLfloat y, FRect *clip = NULL);
		virtual void freeTexture(void);

		bool loadPixelsFromFile8(std::string path);
		bool loadTextureFromPixels8(void);

		void createPixels8(GLuint imgWidth, GLuint imgHeight);
		void copyPixels8(GLubyte *pixels, GLuint imgWidth, GLuint imgHeight);
		void padPixels8(void);
		void blitPixels8(GLuint x, GLuint y, Texture &destination);

		void setPixel8(GLuint x, GLuint y, GLubyte pixel);
		GLubyte getPixel8(GLuint x, GLuint y);
		GLubyte *getPixelData8(void);

		bool loadPixelsFromFile32(std::string path);
		bool loadTextureFromFile32(std::string path);
		bool loadTextureFromFileWithColorKey32(std::string path, GLubyte r, GLubyte g, GLubyte b, GLubyte a = 000);
		bool loadTextureFromPixels32(void);
		bool loadTextureFromPixels32(GLuint *pixels, GLuint imgW, GLuint imgH, GLuint texW, GLuint texH);

		void createPixels32(GLuint imgWidth, GLuint imgHeight);
		void copyPixels32(GLuint *pixels, GLuint imgWidth, GLuint imgHeight);
		void padPixels32(void);
		void blitPixels32(GLuint x, GLuint y, Texture &destination);

		void setPixel32(GLuint x, GLuint y, GLuint pixel);
		GLuint getPixel32(GLuint x, GLuint y);
		GLuint *getPixelData32(void);

		GLuint getTextureID(void);
		GLuint textureWidth(void);
		GLuint textureHeight(void);
		GLuint imageWidth(void);
		GLuint imageHeight(void);

	protected:
		static TexturedPolygon2D *mTexturedPolygon2D;

		void initVBO(void);
		void freeVBO(void);

		GLuint powerOfTwo(GLuint num);

		GLubyte *mPixels8;
		GLuint *mPixels32;
		GLuint mTextureID,
					 mPixelFormat,
					 mTextureWidth,
					 mTextureHeight,
					 mImageWidth,
					 mImageHeight,
					 mVBOID,
					 mIBOID;
};

#endif
