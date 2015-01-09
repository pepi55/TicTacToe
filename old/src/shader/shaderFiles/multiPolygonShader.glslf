#version 130

in vec4 multiColor;
out vec4 LFragment;

void main(void) {
	LFragment = multiColor;
}
