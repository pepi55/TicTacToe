#include "LShaderProgram.hpp"

LShaderProgram::LShaderProgram(void) {
	mProgramID = 0x0;
}

LShaderProgram::~LShaderProgram(void) {
	freeProgram();
}

void LShaderProgram::freeProgram(void) {
	glDeleteProgram(mProgramID);
}

bool LShaderProgram::bind(void) {
	glUseProgram(mProgramID);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		fprintf(stderr, "Error binding shader!\n%s\n", gluErrorString(error));
		printProgramLog(mProgramID);
		return false;
	}

	return true;
}

void LShaderProgram::unbind(void) {
	glUseProgram(0x0);
}

GLuint LShaderProgram::getProgramID(void) {
	return mProgramID;
}

void LShaderProgram::printProgramLog(GLuint program) {
	if (glIsProgram(program)) {
		int infologLength = 0;
		int maxLength = infologLength;
		char *infolog = NULL;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		infolog = new char[maxLength];
		glGetProgramInfoLog(program, maxLength, &infologLength, infolog);

		if (infologLength > 0) {
			fprintf(stdout, "%s\n", infolog);
		}

		delete[] infolog;
	} else {
		fprintf(stderr, "Name %d is not a program!\n", program);
	}
}

void LShaderProgram::printShaderLog(GLuint shader) {
	if (glIsShader(shader)) {
		int infologLength = 0;
		int maxLength = infologLength;
		char *infolog = NULL;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		infolog = new char[maxLength];
		glGetShaderInfoLog(shader, maxLength, &infologLength, infolog);

		if (infologLength > 0) {
			fprintf(stdout, "%s\n", infolog);
		}

		delete[] infolog;
	} else {
		fprintf(stderr, "Name %d is not a shader!\n", shader);
	}
}

GLuint LShaderProgram::loadShaderFromFile(std::string path, GLenum shaderType) {
	GLuint shaderID = 0;

	std::string shaderString;
	std::ifstream sourceFile(path.c_str());

	if (sourceFile) {
		shaderString.assign((std::istreambuf_iterator<char>(sourceFile)), std::istreambuf_iterator<char>());

		shaderID = glCreateShader(shaderType);

		const GLchar *shaderSource = shaderString.c_str();
		glShaderSource(shaderID, 1, (const GLchar **)&shaderSource, NULL);
		glCompileShader(shaderID);

		GLint shaderCompiled = GL_FALSE;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
		if (shaderCompiled != GL_TRUE) {
			fprintf(stderr, "Unable to compile shader %d!\nSource: %s\n", shaderID, shaderSource);
			printShaderLog(shaderID);
			glDeleteShader(shaderID);
			shaderID = 0;
		}
	} else {
		fprintf(stderr, "Unable to open file %s\n", path.c_str());
	}

	return shaderID;
}
