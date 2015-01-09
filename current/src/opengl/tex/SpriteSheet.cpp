#include "SpriteSheet.hpp"
#include "data/VertexData2D.hpp"
#include "data/TexturedVertex2D.hpp"

SpriteSheet::SpriteSheet(void) {
	mVertexDataBuffer = 0x0;
	mIndexBuffers = NULL;
}

SpriteSheet::~SpriteSheet(void) {
	freeSheet();
}

int SpriteSheet::addClipSprite(FRect &newClip) {
	mClips.push_back(newClip);

	return mClips.size() - 1;
}

FRect SpriteSheet::getClip(int index) {
	return mClips[index];
}

bool SpriteSheet::generateDataBuffer(SpriteOrigin origin) {
	if (getTextureID() != 0 && mClips.size() > 0) {
		int totalSprites = mClips.size();
		TexturedVertex2D *vertexData = new TexturedVertex2D[totalSprites * 4];
		mIndexBuffers = new GLuint[totalSprites];

		glGenBuffers(1, &mVertexDataBuffer);
		glGenBuffers(totalSprites, mIndexBuffers);

		GLfloat tW = textureWidth();
		GLfloat tH = textureHeight();
		GLuint spriteIndices[4] = {0, 0, 0, 0};

		GLfloat vTop = 0.0f,
						vBottom = 0.0f,
						vLeft = 0.0f,
						vRight = 0.0f;

		for (int i = 0; i < totalSprites; ++i) {
			spriteIndices[0] = i * 4 + 0;
			spriteIndices[1] = i * 4 + 1;
			spriteIndices[2] = i * 4 + 2;
			spriteIndices[3] = i * 4 + 3;

			switch(origin) {
				case SPRITE_ORIGIN_TOP_LEFT:
					vTop = 0.0f;
					vBottom = mClips[i].h;
					vLeft = 0.0f;
					vRight = mClips[i].w;
					break;

				case SPRITE_ORIGIN_TOP_RIGHT:
					vTop = 0.0f;
					vBottom = mClips[i].h;
					vLeft = -mClips[i].w;
					vRight = 0.0f;
					break;

				case SPRITE_ORIGIN_BOTTOM_LEFT:
					vTop = -mClips[i].h;
					vBottom = 0.0f;
					vLeft = 0.0f;
					vRight = mClips[i].w;
					break;

				case SPRITE_ORIGIN_BOTTOM_RIGHT:
					vTop = -mClips[i].h;
					vBottom = 0.0f;
					vLeft = -mClips[i].w;
					vRight = 0.0f;
					break;

				default:
					vTop = -mClips[i].h / 2.0f;
					vBottom = mClips[i].h / 2.0f;
					vLeft = -mClips[i].w / 2.0f;
					vRight = mClips[i].w / 2.0f;
					break;
			}

			vertexData[spriteIndices[0]].position.x = vLeft;
			vertexData[spriteIndices[0]].position.y = vTop;
			vertexData[spriteIndices[0]].texCoord.s = (mClips[i].x) / tW;
			vertexData[spriteIndices[0]].texCoord.t = (mClips[i].y) / tH;

			vertexData[spriteIndices[1]].position.x = vRight;
			vertexData[spriteIndices[1]].position.y = vTop;
			vertexData[spriteIndices[1]].texCoord.s = (mClips[i].x + mClips[i].w) / tW;
			vertexData[spriteIndices[1]].texCoord.t = (mClips[i].y) / tH;

			vertexData[spriteIndices[2]].position.x = vRight;
			vertexData[spriteIndices[2]].position.y = vBottom;
			vertexData[spriteIndices[2]].texCoord.s = (mClips[i].x + mClips[i].w) / tW;
			vertexData[spriteIndices[2]].texCoord.t = (mClips[i].y + mClips[i].h) / tH;

			vertexData[spriteIndices[3]].position.x = vLeft;
			vertexData[spriteIndices[3]].position.y = vBottom;
			vertexData[spriteIndices[3]].texCoord.s = (mClips[i].x) / tW;
			vertexData[spriteIndices[3]].texCoord.t = (mClips[i].y + mClips[i].h) / tH;

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffers[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), spriteIndices, GL_STATIC_DRAW);
		}

		glBindBuffer(GL_ARRAY_BUFFER, mVertexDataBuffer);
		glBufferData(GL_ARRAY_BUFFER, totalSprites * 4 * sizeof(TexturedVertex2D), vertexData, GL_STATIC_DRAW);

		delete[] vertexData;
	} else {
		if (getTextureID() == 0) {
			fprintf(stderr, "No texture to render with!\n");
		}

		if (mClips.size() <= 0) {
			fprintf(stderr, "No clips to generate vertex data from!\n");
		}

		return false;
	}

	return true;
}

void SpriteSheet::freeSheet(void) {
	if (mVertexDataBuffer != 0x0) {
		glDeleteBuffers(1, &mVertexDataBuffer);
		mVertexDataBuffer = 0x0;
	}

	if (mIndexBuffers != NULL) {
		glDeleteBuffers(mClips.size(), mIndexBuffers);
		delete[] mIndexBuffers;
		mIndexBuffers = NULL;
	}

	mClips.clear();
}

void SpriteSheet::freeTexture(void) {
	freeSheet();

	Texture::freeTexture();
}

void SpriteSheet::renderSprite(int index) {
	if (mVertexDataBuffer != 0x0) {
		glBindTexture(GL_TEXTURE_2D, getTextureID());

		mTexturedPolygon2D->enableVertexPointer();
		mTexturedPolygon2D->enableTexCoordPointer();
			glBindBuffer(GL_ARRAY_BUFFER, mVertexDataBuffer);

			mTexturedPolygon2D->setTexCoordPointer(sizeof(TexturedVertex2D), (GLvoid *)offsetof(TexturedVertex2D, texCoord));
			mTexturedPolygon2D->setVertexPointer(sizeof(TexturedVertex2D), (GLvoid *)offsetof(TexturedVertex2D, position));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffers[index]);
			glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);
		mTexturedPolygon2D->disableVertexPointer();
		mTexturedPolygon2D->disableTexCoordPointer();
	}
}
