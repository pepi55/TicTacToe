#version 130

uniform mat4 LProjectionMatrix;
uniform mat4 LModelViewMatrix;

in vec2 LVertexPos2D;

in vec4 LMultiColor1;
in vec4 LMultiColor2;

out vec4 multiColor;

void main(void) {
	multiColor = LMultiColor1 * LMultiColor2;

	gl_Position = LProjectionMatrix * LModelViewMatrix * vec4(LVertexPos2D.x, LVertexPos2D.y, 0.0, 1.0);
}
