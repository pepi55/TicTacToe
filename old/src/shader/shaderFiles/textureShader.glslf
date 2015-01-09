uniform vec4 LTextureColor;
uniform sampler2D LTextureUnit;

#if __VERSION__ >= 130

in vec2 texCoord;
out vec4 gl_FragColor;

#else

varying vec2 texCoord;

#endif

void main(void) {
	gl_FragColor = texture(LTextureUnit, texCoord) * LTextureColor;
}
