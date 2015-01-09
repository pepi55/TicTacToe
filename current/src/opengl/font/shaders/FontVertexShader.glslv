#version 330

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

in vec2 VertexPos2D;

in vec2 TexCoord;
out vec4 texCoord;

void main(void) {
	texCoord = vec4(TexCoord.s, TexCoord.t, 0.0, 1.0);
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(VertexPos2D.x, VertexPos2D.y, 0.0, 1.0);
}
