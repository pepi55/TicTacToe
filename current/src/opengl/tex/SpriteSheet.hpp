#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include <vector>

#include "Texture.hpp"

enum SpriteOrigin {
	SPRITE_ORIGIN_CENTER,
	SPRITE_ORIGIN_TOP_LEFT,
	SPRITE_ORIGIN_BOTTOM_LEFT,
	SPRITE_ORIGIN_TOP_RIGHT,
	SPRITE_ORIGIN_BOTTOM_RIGHT
};

class SpriteSheet : public Texture {
	public:
		SpriteSheet(void);
		~SpriteSheet(void);

		int addClipSprite(FRect &newClip);
		FRect getClip(int index);
		bool generateDataBuffer(SpriteOrigin origin = SPRITE_ORIGIN_CENTER);
		void freeSheet(void);
		void freeTexture(void);
		void renderSprite(int index);

	protected:
		std::vector<FRect> mClips;
		GLuint mVertexDataBuffer;
		GLuint *mIndexBuffers;
};

#endif
