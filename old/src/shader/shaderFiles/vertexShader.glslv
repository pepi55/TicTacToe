uniform mat4 LProjectionMatrix;
uniform mat4 LModelViewMatrix;

#if __VERSION__ >= 130

in vec2 LVertexPos2D;

in vec3 LMultiColor;
out vec4 multiColor;

#else

attribute vec2 LVertexPos2D;

attribute vec3 LMultiColor;
varying vec4 multiColor;

#endif

void main(void) {
	multiColor = vec4(LMultiColor.r, LMultiColor.g, LMultiColor.b, 1.0);

	gl_Position = LProjectionMatrix * LModelViewMatrix * vec4(LVertexPos2D.x, LVertexPos2D.y, 0.0, 1.0);
}
