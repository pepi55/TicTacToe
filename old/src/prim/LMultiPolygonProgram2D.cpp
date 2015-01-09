#define GLM_FORCE_RADIANS

#include <glm/gtc/type_ptr.hpp>
#include "LMultiPolygonProgram2D.hpp"

LMultiPolygonProgram2D::LMultiPolygonProgram2D(void) {
	mVertexPos2DLocation = 0;
	mMultiColorLocation1 = 0;
	mMultiColorLocation2 = 0;

	mProjectionMatrixLocation = 0;
	mModelViewMatrixLocation = 0;
}

bool LMultiPolygonProgram2D::loadProgram(void) {
	mProgramID = glCreateProgram();

	GLuint vertexShader = loadShaderFromFile("src/shader/shaderFiles/multiPolygonShader.glslv", GL_VERTEX_SHADER);
	GLuint fragmentShader = loadShaderFromFile("src/shader/shaderFiles/multiPolygonShader.glslf", GL_FRAGMENT_SHADER);

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
	glDeleteShader(vertexShader);

	const GLchar *var = "LVertexPos2D";

	mVertexPos2DLocation = glGetAttribLocation(mProgramID, var);
	if (mVertexPos2DLocation == -1) {
		fprintf(stderr, "%s is not a valid glsl program variable!\n", var);
	}

	var = "LMultiColor1";

	mMultiColorLocation1 = glGetAttribLocation(mProgramID, var);
	if (mMultiColorLocation1 == -1) {
		fprintf(stderr, "%s is not a valid glsl program variable!\n", var);
	}

	var = "LMultiColor2";

	mMultiColorLocation2 = glGetAttribLocation(mProgramID, var);
	if (mMultiColorLocation2 == -1) {
		fprintf(stderr, "%s is not a valid glsl program variable!\n", var);
	}

	var = "LProjectionMatrix";

	mProjectionMatrixLocation = glGetUniformLocation(mProgramID, var);
	if (mProjectionMatrixLocation == -1) {
		fprintf(stderr, "%s is not a valid glsl program variable!\n", var);
	}

	var = "LModelViewMatrix";

	mModelViewMatrixLocation = glGetUniformLocation(mProgramID, var);
	if (mModelViewMatrixLocation == -1) {
		fprintf(stderr, "%s is not a valid glsl program variable!\n", var);
	}

	return true;
}

void LMultiPolygonProgram2D::setVertexPointer(GLsizei stride, const GLvoid *data) {
	glVertexAttribPointer(mVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, stride, data);
}

void LMultiPolygonProgram2D::setColorPointer(GLsizei stride, const GLvoid *data) {
	glVertexAttribPointer(mMultiColorLocation1, 4, GL_FLOAT, GL_FALSE, stride, data);
}

void LMultiPolygonProgram2D::setColor2Pointer(GLsizei stride, const GLvoid *data) {
	glVertexAttribPointer(mMultiColorLocation2, 4, GL_FLOAT, GL_FALSE, stride, data);
}

void LMultiPolygonProgram2D::enableDataPointers(void) {
	glEnableVertexAttribArray(mVertexPos2DLocation);

	glEnableVertexAttribArray(mMultiColorLocation1);
	glEnableVertexAttribArray(mMultiColorLocation2);
}

void LMultiPolygonProgram2D::disableDataPointers(void) {
	glDisableVertexAttribArray(mMultiColorLocation1);
	glDisableVertexAttribArray(mMultiColorLocation2);

	glDisableVertexAttribArray(mVertexPos2DLocation);
}

void LMultiPolygonProgram2D::setProjection(glm::mat4 matrix) {
	mProjectionMatrix = matrix;
}

void LMultiPolygonProgram2D::setModelView(glm::mat4 matrix) {
	mModelViewMatrix = matrix;
}

void LMultiPolygonProgram2D::leftMultProjection(glm::mat4 matrix) {
	mProjectionMatrix = matrix * mProjectionMatrix;
}

void LMultiPolygonProgram2D::leftMultModelView(glm::mat4 matrix) {
	mModelViewMatrix = matrix * mModelViewMatrix;
}

void LMultiPolygonProgram2D::updateProjection(void) {
	glUniformMatrix4fv(mProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
}

void LMultiPolygonProgram2D::updateModelView(void) {
	glUniformMatrix4fv(mModelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(mModelViewMatrix));
}
