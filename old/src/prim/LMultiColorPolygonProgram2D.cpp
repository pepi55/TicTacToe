#define GLM_FORCE_RADIANS

#include <glm/gtc/type_ptr.hpp>
#include "LMultiColorPolygonProgram2D.hpp"

LMultiColorPolygonProgram2D::LMultiColorPolygonProgram2D(void) {
	mVertexPos2DLocation = 0;
	mMultiColorLocation = 0;

	mProjectionMatrixLocation = 0;
	mModelViewMatrixLocation = 0;
}

bool LMultiColorPolygonProgram2D::loadProgram(void) {
	mProgramID = glCreateProgram();

	GLuint vertexShader = loadShaderFromFile("src/shader/shaderFiles/vertexShader.glslv", GL_VERTEX_SHADER);
	GLuint fragmentShader = loadShaderFromFile("src/shader/shaderFiles/fragmentShader.glslf", GL_FRAGMENT_SHADER);

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

	var = "LMultiColor";

	mMultiColorLocation = glGetAttribLocation(mProgramID, var);
	if (mMultiColorLocation == -1) {
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

void LMultiColorPolygonProgram2D::setVertexPointer(GLsizei stride, const GLvoid *data) {
	glVertexAttribPointer(mVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, stride, data);
}

void LMultiColorPolygonProgram2D::setColorPointer(GLsizei stride, const GLvoid *data) {
	glVertexAttribPointer(mMultiColorLocation, 4, GL_FLOAT, GL_FALSE, stride, data);
}

void LMultiColorPolygonProgram2D::enableVertexPointer(void) {
	glEnableVertexAttribArray(mVertexPos2DLocation);
}

void LMultiColorPolygonProgram2D::disableVertexPointer(void) {
	glDisableVertexAttribArray(mVertexPos2DLocation);
}

void LMultiColorPolygonProgram2D::enableColorPointer(void) {
	glEnableVertexAttribArray(mMultiColorLocation);
}

void LMultiColorPolygonProgram2D::disableColorPointer(void) {
	glDisableVertexAttribArray(mMultiColorLocation);
}

void LMultiColorPolygonProgram2D::setProjection(glm::mat4 matrix) {
	mProjectionMatrix = matrix;
}

void LMultiColorPolygonProgram2D::setModelView(glm::mat4 matrix) {
	mModelViewMatrix = matrix;
}

void LMultiColorPolygonProgram2D::leftMultProjection(glm::mat4 matrix) {
	mProjectionMatrix = matrix * mProjectionMatrix;
}

void LMultiColorPolygonProgram2D::leftMultModelView(glm::mat4 matrix) {
	mModelViewMatrix = matrix * mModelViewMatrix;
}

void LMultiColorPolygonProgram2D::updateProjection(void) {
	glUniformMatrix4fv(mProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
}

void LMultiColorPolygonProgram2D::updateModelView(void) {
	glUniformMatrix4fv(mModelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(mModelViewMatrix));
}
