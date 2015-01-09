#if __VERSION__ >= 130

in vec4 multiColor;
out vec4 gl_FragColor;

#else

varying vec4 multiColor;

#endif

uniform vec4 LPolygonColor = vec4(1.0, 1.0, 1.0, 1.0);

void main(void) {
	gl_FragColor = multiColor;
}
