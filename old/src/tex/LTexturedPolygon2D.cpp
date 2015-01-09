#define GLM_FORCE_RADIANS

#include <glm/gtc/type_ptr.hpp>
#include "LTexturedPolygon2D.hpp"

LTexturedPolygon2D::LTexturedPolygon2D(void) {
	mVertexPos2DLocation = 0;
	mTexCoordLocation = 0;

	mProjectionMatrixLocation = 0;
	mModelViewMatrixLocation = 0;
	mTextureColorLocation = 0;
	mTextureUnitLocation = 0;
}

bool LTexturedPolygon2D::loadProgram(void) {
	mProgramID = glCreateProgram();

	GLuint vertexShader = loadShaderFromFile("src/shader/shaderFiles/textureShader.glslv", GL_VERTEX_SHADER);
	GLuint fragmentShader = loadShaderFromFile("src/shader/shaderFiles/textureShader.glslf", GL_FRAGMENT_SHADER);

	if (vertexShader == 0) {
		glDeleteProgram(mProgramID);
		mProgramID = 0;

		return false;
	}

	glAttachShader(mProgramID, vertexShader);

	if (fragmentShader == 0) {
		glDeleteShader(vertexShader);
		glDeleteProgram(mProgramID);
		mProgramID = 0;

		return false;
	}

	glAttachShader(mProgramID, fragmentShader);
	glLinkProgram(mProgramID);

	GLint programSuccess = GL_TRUE;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE) {
		fprintf(stderr, "Error linking program %d!\n", mProgramID);
		printProgramLog(mProgramID);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(mProgramID);
		mProgramID = 0;

		return false;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	const GLchar *var = "LVertexPos2D";

	mVertexPos2DLocation = glGetAttribLocation(mProgramID, var);
	if (mVertexPos2DLocation == -1) {
		fprintf(stderr, "%s is not a valid glsl program variable!\n", var);
	}

	var = "LTexCoord";

	mTexCoordLocation = glGetAttribLocation(mProgramID, var);
	if (mTexCoordLocation == -1) {
		fprintf(stderr, "%s is not a valid glsl program variable!\n", var);
	}

	var = "LTextureColor";

	mTextureColorLocation = glGetUniformLocation(mProgramID, var);
	if (mTextureColorLocation == -1) {
		fprintf(stderr, "%s is not a valid glsl program variable!\n", var);
	}

	var = "LTextureUnit";

	mTextureUnitLocation = glGetUniformLocation(mProgramID, var);
	if (mTextureUnitLocation == -1) {
		fprintf(stderr, "%s is not a valid glsl program variable!\n", var);
	}

	var = "LProjectionMatrix";

	mProjectionMatrixLocation = glGetUniformLocation(mProgramID, var);
	if (mProjectionMatrixLocation == -1) {
		fprintf(stderr, "%s is not a valid glsl program variable!\n", var);
	}

	var = "LModelViewMatrix";

	mModelViewMatrixLocation = glGetUniformLocation(mProgramID, var);
	if (mProjectionMatrixLocation == -1) {
		fprintf(stderr, "%s is not a valid glsl program variable!\n", var);
	}

	return true;
}

void LTexturedPolygon2D::setVertexPointer(GLsizei stride, const GLvoid *data) {
	glVertexAttribPointer(mVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, stride, data);
}

void LTexturedPolygon2D::setTexCoordPointer(GLsizei stride, const GLvoid *data) {
	glVertexAttribPointer(mTexCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, data);
}

void LTexturedPolygon2D::enableVertexPointer(void) {
	glEnableVertexAttribArray(mVertexPos2DLocation);
}

void LTexturedPolygon2D::disableVertexPointer(void) {
	glDisableVertexAttribArray(mVertexPos2DLocation);
}

void LTexturedPolygon2D::enableTexCoordPointer(void) {
	glEnableVertexAttribArray(mTexCoordLocation);
}

void LTexturedPolygon2D::disableTexCoordPointer(void) {
	glDisableVertexAttribArray(mTexCoordLocation);
}

void LTexturedPolygon2D::setProjection(glm::mat4 matrix) {
	mProjectionMatrix = matrix;
}

void LTexturedPolygon2D::setModelView(glm::mat4 matrix) {
	mModelViewMatrix = matrix;
}

void LTexturedPolygon2D::leftMultProjection(glm::mat4 matrix) {
	mProjectionMatrix = matrix * mProjectionMatrix;
}

void LTexturedPolygon2D::leftMultModelView(glm::mat4 matrix) {
	mModelViewMatrix = matrix * mModelViewMatrix;
}

void LTexturedPolygon2D::updateProjection(void) {
	glUniformMatrix4fv(mProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
}

void LTexturedPolygon2D::updateModelView(void) {
	glUniformMatrix4fv(mModelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(mModelViewMatrix));
}

void LTexturedPolygon2D::setTextureColor(LColorRGBA color) {
	glUniform4f(mTextureColorLocation, color.r, color.g, color.b, color.a);
}

void LTexturedPolygon2D::setTextureUnit(GLuint unit) {
	glUniform1i(mTextureUnitLocation, unit);
}
