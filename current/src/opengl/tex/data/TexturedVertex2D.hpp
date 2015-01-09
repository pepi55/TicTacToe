#ifndef TEXTURED_VERTEX_2D_H
#define TEXTURED_VERTEX_2D_H

#include "VertexPos2D.hpp"
#include "TexCoord.hpp"

struct TexturedVertex2D {
	VertexPos2D position;
	TexCoord texCoord;
};

#endif
