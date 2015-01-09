uniform mat4 LProjectionMatrix;
uniform mat4 LModelViewMatrix;

#if __VERSION__ >= 130

in vec2 LVertexPos2D;

in vec2 LTexCoord;
out vec2 texCoord;

#else

attribute vec2 LVertexPos2D;

attribute vec2 LTexCoord;
varying vec2 texCoord;

#endif

void main(void) {
	texCoord = LTexCoord;

	gl_Position = LProjectionMatrix * LModelViewMatrix * vec4(LVertexPos2D.x, LVertexPos2D.y, 0.0, 1.0);
}
