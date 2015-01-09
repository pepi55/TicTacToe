#ifndef LTEXTURED_VERTEX_2D_H
#define LTEXTURED_VERTEX_2D_H

#include "LVertexPos2D.hpp"
#include "../tex/LTexCoord.hpp"

struct LTexturedVertex2D {
	LVertexPos2D position;
	LTexCoord texCoord;
};

#endif
