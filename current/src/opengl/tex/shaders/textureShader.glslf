#version 330

uniform vec4 TextureColor;
uniform sampler2D TextureUnit;

in vec2 texCoord;
out vec4 gl_FragColor;

void main(void) {
	gl_FragColor = texture(TextureUnit, texCoord) * TextureColor;
}
