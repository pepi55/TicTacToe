#define GLM_FORCE_RADIANS

#include <string.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "LTexture.hpp"
#include "../vertex/LTexturedVertex2D.hpp"
//#include "../vertex/LVertexData2D.hpp"

GLenum DEFAULT_TEXTURE_WRAP = GL_REPEAT;
LTexturedPolygon2D *LTexture::mTexturedPolygon2D = NULL;

LTexture::LTexture(void) {
	mTextureID = 0;
	mPixels32 = NULL;
	mPixels8 = NULL;
	mPixelFormat = 0x0;

	mTextureWidth = 0;
	mTextureHeight = 0;

	mImageWidth = 0;
	mImageHeight = 0;

	mVBOID = 0;
	mIBOID = 0;
}

LTexture::~LTexture(void) {
	freeTexture();

	freeVBO();
}

void LTexture::setTexturedPolygon2D(LTexturedPolygon2D *program) {
	mTexturedPolygon2D = program;
}

bool LTexture::lock(void) {
	if (mPixels32 == NULL && mPixels8 == NULL && mTextureID != 0) {
		GLuint size = mTextureWidth * mTextureHeight;

		if (mPixelFormat == GL_RGBA) {
			mPixels32 = new GLuint[size];
		} else if (mPixelFormat == GL_ALPHA) {
			mPixels8 = new GLubyte[size];
		}

		glBindTexture(GL_TEXTURE_2D, mTextureID);
		glGetTexImage(GL_TEXTURE_2D, 0, mPixelFormat, GL_UNSIGNED_BYTE, mPixels32);
		glBindTexture(GL_TEXTURE_2D, 0x0);

		return true;
	}

	return false;
}

bool LTexture::unlock(void) {
	if ((mPixels32 != NULL || mPixels8 != NULL) && mTextureID != 0) {
		glBindTexture(GL_TEXTURE_2D, mTextureID);

		void *pixels = (mPixelFormat == GL_RGBA) ? (void*)mPixels32 : (void*)mPixels8;

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mTextureWidth, mTextureHeight,
				mPixelFormat, GL_UNSIGNED_BYTE, pixels
				);

		if (mPixels32 != NULL) {
			delete[] mPixels32;
			mPixels32 = NULL;
		} else if (mPixels8 != NULL) {
			delete[] mPixels8;
			mPixels8 = NULL;
		}

		glBindTexture(GL_TEXTURE_2D, 0x0);

		return true;
	}

	return false;
}

void LTexture::initVBO(void) {
	if (mTextureID != 0 && mVBOID == 0) {
		LTexturedVertex2D vData[4];
		GLuint iData[4];

		for (int i = 0; i < 4; ++i) {
			iData[i] = i;
		}

		glGenBuffers(1, &mVBOID);
		glBindBuffer(GL_ARRAY_BUFFER, mVBOID);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(LTexturedVertex2D), vData, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &mIBOID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBOID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), iData, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0x0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0x0);
	}
}

void LTexture::freeVBO(void) {
	if (mVBOID != 0) {
		glDeleteBuffers(1, &mVBOID);
		glDeleteBuffers(1, &mIBOID);
	}
}

void LTexture::render(GLfloat x, GLfloat y, LFRect *clip) {
	if (mTextureID != 0) {
		GLfloat texTop = 0.0f;
		GLfloat	texBottom = (GLfloat)mImageHeight / (GLfloat)mTextureHeight;
		GLfloat texLeft = 0.0f;
		GLfloat texRight = (GLfloat)mImageWidth / (GLfloat)mTextureWidth;

		GLfloat quadWidth = mImageWidth;
		GLfloat quadHeight = mImageHeight;

		if (clip != NULL) {
			texLeft = clip->x / mTextureWidth;
			texRight = (clip->x + clip->w) / mTextureWidth;
			texTop = clip->y / mTextureHeight;
			texBottom = (clip->y + clip->h) / mTextureHeight;

			quadWidth = clip->w;
			quadHeight = clip->h;
		}

		mTexturedPolygon2D->leftMultModelView(glm::translate(glm::vec3(x, y, 0.0f)));
		mTexturedPolygon2D->updateModelView();

		LTexturedVertex2D vData[4];

		vData[0].texCoord.s = texLeft;		vData[0].texCoord.t = texTop;
		vData[1].texCoord.s = texRight;		vData[1].texCoord.t = texTop;
		vData[2].texCoord.s = texRight;		vData[2].texCoord.t = texBottom;
		vData[3].texCoord.s = texLeft;		vData[3].texCoord.t = texBottom;

		vData[0].position.x = 0.0f;				vData[0].position.y = 0.0f;
		vData[1].position.x = quadWidth;	vData[1].position.y = 0.0f;
		vData[2].position.x = quadWidth;	vData[2].position.y = quadHeight;
		vData[3].position.x = 0.0f;				vData[3].position.y = quadHeight;

		glBindTexture(GL_TEXTURE_2D, mTextureID);
		mTexturedPolygon2D->enableVertexPointer();
		mTexturedPolygon2D->enableTexCoordPointer();
			glBindBuffer(GL_ARRAY_BUFFER, mVBOID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(LTexturedVertex2D), vData);

			mTexturedPolygon2D->setTexCoordPointer(sizeof(LTexturedVertex2D), (GLvoid *)offsetof(LTexturedVertex2D, texCoord));
			mTexturedPolygon2D->setVertexPointer(sizeof(LTexturedVertex2D), (GLvoid *)offsetof(LTexturedVertex2D, position));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBOID);
			glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);
		mTexturedPolygon2D->disableVertexPointer();
		mTexturedPolygon2D->disableTexCoordPointer();
	}
}

void LTexture::freeTexture(void) {
	if (mTextureID != 0) {
		glDeleteTextures(1, &mTextureID);
		mTextureID = 0;
	}

	if (mPixels32 != NULL) {
		delete[] mPixels32;
		mPixels32 = NULL;
	}

	if (mPixels8 != NULL) {
		delete[] mPixels8;
		mPixels8 = NULL;
	}

	mImageWidth = 0;
	mImageHeight = 0;
	mTextureWidth = 0;
	mTextureHeight = 0;

	mPixelFormat = 0x0;
}

bool LTexture::loadPixelsFromFile8(std::string path) {
	freeTexture();

	bool pixelsLoaded = false;
	ILboolean success = false;
	ILuint imgID = 0;

	ilGenImages(1, &imgID);
	ilBindImage(imgID);

	success = ilLoadImage(path.c_str());

	if (success == IL_TRUE) {
		success = ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);

		if (success == IL_TRUE) {
			GLuint imgWidth = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
			GLuint imgHeight = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);

			GLuint texWidth = powerOfTwo(imgWidth);
			GLuint texHeight = powerOfTwo(imgHeight);

			if (imgWidth != texWidth || imgHeight != texHeight) {
				iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);
				iluEnlargeCanvas((int)texWidth, (int)texHeight, 1);
			}

			GLuint size = texWidth * texHeight;
			mPixels8 = new GLubyte[size];

			mImageWidth = imgWidth;
			mImageHeight = imgHeight;
			mTextureWidth = texWidth;
			mTextureHeight = texHeight;

			memcpy(mPixels8, ilGetData(), size);
			pixelsLoaded = true;
		}

		ilDeleteImages(1, &imgID);
		mPixelFormat = GL_ALPHA;
	}

	if (!pixelsLoaded) {
		fprintf(stderr, "Unable to load %s\n", path.c_str());
	}

	return pixelsLoaded;
}

bool LTexture::loadTextureFromPixels8(void) {
	bool success = true;

	if (mTextureID == 0 && mPixels8 != NULL) {
		glGenTextures(1, &mTextureID);
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, mTextureWidth, mTextureHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, mPixels8);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, DEFAULT_TEXTURE_WRAP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, DEFAULT_TEXTURE_WRAP);

		glBindTexture(GL_TEXTURE_2D, 0x0);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			fprintf(stderr, "Error loading texture from %p pixels!\n%s\n", (void*)mPixels8, gluErrorString(error));
			success = false;
		} else {
			delete[] mPixels8;
			mPixels8 = NULL;

			initVBO();

			mPixelFormat = GL_ALPHA;
		}
	} else {
		fprintf(stderr, "Cannot load texture from current pixels!\n");

		if (mTextureID != 0) {
			fprintf(stderr, "A texture is already loaded!\n");
		}

		if (mPixels8 == NULL) {
			fprintf(stderr, "No pixels to load texture from!\n");
		}
	}

	return success;
}

void LTexture::createPixels8(GLuint imgWidth, GLuint imgHeight) {
	if (imgWidth > 0 && imgHeight > 0) {
		freeTexture();

		GLuint size = imgWidth * imgHeight;
		mPixels8 = new GLubyte[size];
		memset(mPixels8, 0, size);

		mImageWidth = imgWidth;
		mImageHeight = imgHeight;
		mTextureWidth = imgWidth;
		mTextureHeight = imgHeight;

		mPixelFormat = GL_ALPHA;
	}
}

void LTexture::copyPixels8(GLubyte *pixels, GLuint imgWidth, GLuint imgHeight) {
	if (imgWidth > 0 && imgHeight > 0) {
		freeTexture();

		GLuint size = imgWidth * imgHeight;
		mPixels8 = new GLubyte[size];
		memcpy(mPixels8, pixels, size);

		mImageWidth = imgWidth;
		mImageHeight = imgHeight;
		mTextureWidth = imgWidth;
		mTextureHeight = imgHeight;

		mPixelFormat = GL_ALPHA;
	}
}

void LTexture::padPixels8(void) {
	if (mPixels8 != NULL) {
		GLuint oTextureWidth = mTextureWidth;
		//GLuint oTextureHeight = mTextureHeight;

		mTextureWidth = powerOfTwo(mImageWidth);
		mTextureHeight = powerOfTwo(mImageHeight);

		if (mTextureWidth != mImageWidth || mTextureHeight != mImageHeight) {
			GLuint size = mTextureWidth * mTextureHeight;
			GLubyte *pixels = new GLubyte[size];

			for (int i = 0; i < (int)mImageHeight; ++i) {
				memcpy(&pixels[i * mTextureWidth], &mPixels8[i * oTextureWidth], mImageWidth);
			}

			delete[] mPixels8;
			mPixels8 = pixels;
		}
	}
}

void LTexture::blitPixels8(GLuint x, GLuint y, LTexture &destination) {
	if (mPixels8 != NULL && destination.mPixels8 != NULL) {
		GLubyte *dPixels = destination.mPixels8;
		GLubyte *sPixels = mPixels8;

		for (int i = 0; i < (int)mImageHeight; ++i) {
			memcpy(&dPixels[(i + y) * destination.mTextureWidth + x], &sPixels[i * mTextureWidth], mImageWidth);
		}
	}
}

bool LTexture::loadPixelsFromFile32(std::string path) {
	freeTexture();

	bool pixelsLoaded = false;
	ILuint imgID = 0;
	ILboolean success;

	ilGenImages(1, &imgID);
	ilBindImage(imgID);
	success = ilLoadImage(path.c_str());

	if (success == IL_TRUE) {
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (success == IL_TRUE) {
			GLuint imgWidth = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
			GLuint imgHeight = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);

			GLuint texWidth = powerOfTwo(imgWidth);
			GLuint texHeight = powerOfTwo(imgHeight);

			if (imgWidth != texWidth || imgHeight != texHeight) {
				iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);
				iluEnlargeCanvas((int)texWidth, (int)texHeight, 1);
			}

			GLuint size = texWidth * texHeight;
			mPixels32 = new GLuint[size];

			mImageWidth = imgWidth;
			mImageHeight = imgHeight;
			mTextureWidth = texWidth;
			mTextureHeight = texHeight;

			memcpy(mPixels32, ilGetData(), size * 4);
			pixelsLoaded = true;
		}

		ilDeleteImages(1, &imgID);
		mPixelFormat = GL_RGBA;
	}

	if (!pixelsLoaded) {
		fprintf(stderr, "Unable to load %s\n", path.c_str());
	}

	return pixelsLoaded;
}

bool LTexture::loadTextureFromFile32(std::string path) {
	bool textureLoaded = false;

	ILuint imgID = 0;
	ILboolean success;

	ilGenImages(1, &imgID);
	ilBindImage(imgID);

	success = ilLoadImage(path.c_str());

	if (success == IL_TRUE) {
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (success == IL_TRUE) {
			GLuint imgWidth = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
			GLuint imgHeight = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);

			GLuint texWidth = powerOfTwo(imgWidth);
			GLuint texHeight = powerOfTwo(imgHeight);

			if (imgWidth != texWidth || imgHeight != texHeight) {
				iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);
				iluEnlargeCanvas((int)texWidth, (int)texHeight, 1);
			}

			textureLoaded = loadTextureFromPixels32((GLuint*)ilGetData(),
					imgWidth, imgHeight,
					texWidth, texHeight
					);
		}

		ilDeleteImages(1, &imgID);
		mPixelFormat = GL_RGBA;
	}

	if (!textureLoaded) {
		fprintf(stderr, "Unable to load image %s\n", path.c_str());
	}

	return textureLoaded;
}

bool LTexture::loadTextureFromFileWithColorKey32(std::string path, GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
	if (!loadPixelsFromFile32(path)) {
		return false;
	}

	GLuint size = mTextureWidth * mTextureHeight;
	for (GLuint i = 0; i < size; ++i) {
		GLubyte *colors = (GLubyte*)&mPixels32[i];

		if (colors[0] == r && colors[1] == g && colors[2] == b && (0 == a || colors[3] == a)) {
			colors[0] = 255;
			colors[1] = 255;
			colors[2] = 255;
			colors[3] = 000;
		}
	}

	return loadTextureFromPixels32();
}

bool LTexture::loadTextureFromPixels32(void) {
	bool success = true;

	if (mTextureID == 0 && mPixels32 != NULL) {
		glGenTextures(1, &mTextureID);
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mPixels32);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, DEFAULT_TEXTURE_WRAP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, DEFAULT_TEXTURE_WRAP);

		glBindTexture(GL_TEXTURE_2D, 0x0);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			fprintf(stderr, "Error loading texture from %p pixels!\n%s\n", (void*)mPixels32, gluErrorString(error));
			success = false;
		} else {
			delete[] mPixels32;
			mPixels32 = NULL;

			initVBO();

			mPixelFormat = GL_RGBA;
		}
	} else {
		fprintf(stderr, "Cannot load texture from pixels!\n");

		if (mTextureID != 0) {
			fprintf(stderr, "A texture has already been loaded!\n");
		} else if (mPixels32 == NULL) {
			fprintf(stderr, "No pixels to create texture from!\n");
		}

		success = false;
	}

	return success;
}

bool LTexture::loadTextureFromPixels32(GLuint *pixels, GLuint imgW, GLuint imgH, GLuint texW, GLuint texH) {
	freeTexture();

	mImageWidth = imgW;
	mImageHeight = imgH;
	mTextureWidth = texW;
	mTextureHeight = texH;

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, DEFAULT_TEXTURE_WRAP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, DEFAULT_TEXTURE_WRAP);

	glBindTexture(GL_TEXTURE_2D, 0x0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		fprintf(stderr, "Error loading texture from %p pixels!\n%s\n", (void*)pixels, gluErrorString(error));
		return false;
	}

	initVBO();

	mPixelFormat = GL_RGBA;

	return true;
}

void LTexture::createPixels32(GLuint imgWidth, GLuint imgHeight) {
	if (imgWidth > 0 && imgHeight > 0) {
		freeTexture();

		GLuint size = imgWidth * imgHeight;
		mPixels32 = new GLuint[size];

		mImageWidth = imgWidth;
		mImageHeight = imgHeight;
		mTextureWidth = mImageWidth;
		mTextureHeight = mImageHeight;

		mPixelFormat = GL_RGBA;
	}
}

void LTexture::copyPixels32(GLuint *pixels, GLuint imgWidth, GLuint imgHeight) {
	if (imgWidth > 0 && imgHeight > 0) {
		freeTexture();

		GLuint size = imgWidth * imgHeight;
		mPixels32 = new GLuint[size];
		memcpy(mPixels32, pixels, size * 4);

		mImageWidth = imgWidth;
		mImageHeight = imgHeight;
		mTextureWidth = mImageWidth;
		mTextureHeight = mImageHeight;

		mPixelFormat = GL_RGBA;
	}
}

void LTexture::padPixels32(void) {
	if (mPixels32 != NULL) {
		GLuint oTextureWidth = mTextureWidth;
		//GLuint oTextureHeight = mTextureHeight;

		mTextureWidth = powerOfTwo(mImageWidth);
		mTextureHeight = powerOfTwo(mImageHeight);

		if (mTextureWidth != mImageWidth || mTextureHeight != mImageHeight) {
			GLuint size = mTextureWidth * mTextureHeight;
			GLuint *pixels = new GLuint[size];

			for (int i = 0; i < (int)mImageHeight; ++i) {
				memcpy(&pixels[i * mTextureWidth], &mPixels32[i * oTextureWidth], mTextureWidth * 4);
			}

			delete[] mPixels32;
			mPixels32 = pixels;
		}
	}
}

void LTexture::blitPixels32(GLuint x, GLuint y, LTexture &destination) {
	if (mPixels32 != NULL && destination.mPixels32 != NULL) {
		GLuint *dPixels = destination.mPixels32;
		GLuint *sPixels = mPixels32;

		for (int i = 0; i < (int)mImageHeight; ++i) {
			memcpy(&dPixels[(i + y) * destination.mTextureWidth + x], &sPixels[i * mTextureWidth], mImageWidth * 4);
		}
	}
}

GLuint LTexture::powerOfTwo(GLuint num) {
	if (num != 0) {
		num--;
		num |= (num >> 1);
		num |= (num >> 2);
		num |= (num >> 4);
		num |= (num >> 8);
		num |= (num >> 16);
		num++;
	}

	return num;
}

void LTexture::setPixel8(GLuint x, GLuint y, GLubyte pixel) {
	mPixels8[y * mTextureWidth + x] = pixel;
}

GLubyte LTexture::getPixel8(GLuint x, GLuint y) {
	return mPixels8[y * mTextureWidth + x];
}

GLubyte *LTexture::getPixelData8(void) {
	return mPixels8;
}

void LTexture::setPixel32(GLuint x, GLuint y, GLuint pixel) {
	mPixels32[y * mTextureWidth + x] = pixel;
}

GLuint LTexture::getPixel32(GLuint x, GLuint y) {
	return mPixels32[y * mTextureWidth + x];
}

GLuint *LTexture::getPixelData32(void) {
	return mPixels32;
}

GLuint LTexture::getTextureID(void) {
	return mTextureID;
}

GLuint LTexture::textureWidth(void) {
	return mTextureWidth;
}

GLuint LTexture::textureHeight(void) {
	return mTextureHeight;
}

GLuint LTexture::imageWidth(void) {
	return mImageWidth;
}

GLuint LTexture::imageHeight(void) {
	return mImageHeight;
}
