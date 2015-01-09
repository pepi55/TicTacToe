#ifndef LVERTEX_DATA_2D_H
#define LVERTEX_DATA_2D_H

#include "../vertex/LVertexPos2D.hpp"
#include "../tex/LTexCoord.hpp"

struct LVertexData2D {
	LVertexPos2D position;
	LTexCoord texCoord;
};

#endif
