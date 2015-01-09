#ifndef LMULTI_COLOR_VERTEX_2D_H
#define LMULTI_COLOR_VERTEX_2D_H

#include "LVertexPos2D.hpp"
#include "../shader/LColorRGBA.hpp"

struct LMultiColorVertex2D {
	LVertexPos2D pos;
	LColorRGBA rgba;
};

#endif
