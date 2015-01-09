#version 330

uniform vec4 TextColor;
uniform sampler2D TextureUnit;

in vec4 texCoord;
out vec4 gl_FragColor;

void main(void) {
	vec4 red = texture(TextureUnit, texCoord.st);
	gl_FragColor = vec4(1.0, 1.0, 1.0, red.r) * TextColor;
}
