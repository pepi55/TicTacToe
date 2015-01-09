#define GLM_FORCE_RADIANS

#include <glm/gtc/type_ptr.hpp>
#include "LPlainPolygonProgram2D.hpp"

LPlainPolygonProgram2D::LPlainPolygonProgram2D(void) {
	mPolygonColorLocation = 0;
	mProjectionMatrixLocation = 0;
	mModelViewMatrixLocation = 0;
}

bool LPlainPolygonProgram2D::loadProgram(void) {
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
	glDeleteShader(fragmentShader);

	const GLchar *var = "LPolygonColor";
	mPolygonColorLocation = glGetUniformLocation(mProgramID, var);
	if (mPolygonColorLocation == -1) {
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

void LPlainPolygonProgram2D::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	glUniform4f(mPolygonColorLocation, r, g, b, a);
}

void LPlainPolygonProgram2D::setProjection(glm::mat4 matrix) {
	mProjectionMatrix = matrix;
}

void LPlainPolygonProgram2D::setModelView(glm::mat4 matrix) {
	mModelViewMatrix = matrix;
}

void LPlainPolygonProgram2D::leftMultProjection(glm::mat4 matrix) {
	mProjectionMatrix = matrix * mProjectionMatrix;
}

void LPlainPolygonProgram2D::leftMultModelView(glm::mat4 matrix) {
	mModelViewMatrix = matrix * mModelViewMatrix;
}

void LPlainPolygonProgram2D::updateProjection(void) {
	glUniformMatrix4fv(mProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
}

void LPlainPolygonProgram2D::updateModelView(void) {
	glUniformMatrix4fv(mModelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(mModelViewMatrix));
}
