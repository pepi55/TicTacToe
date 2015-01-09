#define GLM_FORCE_RADIANS

#include <IL/il.h>
#include <IL/ilu.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "LUtil.hpp"

GLfloat gCameraX = 0.0f,
				gCameraY = 0.0f;

LMultiPolygonProgram2D gMultiPolygon;

GLuint gVertexVBO = 0x0,
			 gRGBYVBO = 0x0,
			 gCYMWVBO = 0x0,
			 gGrayVBO = 0x0,
			 gIBO = 0x0;

GLuint gLeftVAO = 0x0,
			 gRightVAO = 0x0;

bool initGL(void) {
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		fprintf(stderr, "Error initializing GLEW!\n%s\n", glewGetErrorString(glewError));
		return false;
	}

	if (!GLEW_VERSION_3_1) {
		fprintf(stderr, "OpenGL 3.1 not supported!\n");
		return false;
	}

	glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		fprintf(stderr, "Error initializing OpenGL!\n%s\n", gluErrorString(error));
		return false;
	}

	ilInit();
	iluInit();
	ilClearColor(255, 255, 255, 000);

	ILenum ilError = ilGetError();
	if (ilError != IL_NO_ERROR) {
		fprintf(stderr, "Unable to initialize DevIL!\n%s\n", iluErrorString(ilError));
		return false;
	}

	if (!LFont::initFreetype()) {
		fprintf(stderr, "Unable to initialize Truetype!\n");
	}

	return true;
}

bool loadGP(void) {
	if (!gMultiPolygon.loadProgram()) {
		fprintf(stderr, "Unable to load shader!\n");
		return false;
	}

	gMultiPolygon.bind();

	gMultiPolygon.setProjection(glm::ortho<GLfloat>(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f, -1.0f));
	gMultiPolygon.updateProjection();

	gMultiPolygon.setModelView(glm::mat4());
	gMultiPolygon.updateProjection();

	return true;
}

bool loadMedia(void) {
	LVertexPos2D quadPos[4];
	LColorRGBA quadColorRGBY[4];
	LColorRGBA quadColorCYMW[4];
	LColorRGBA quadColorGray[4];
	GLuint indices[4];

	quadPos[0].x = -50.0f;
	quadPos[0].y = -50.0f;

	quadPos[1].x = 50.0f;
	quadPos[1].y = -50.0f;

	quadPos[2].x = 50.0f;
	quadPos[2].y = 50.0f;

	quadPos[3].x = -50.0f;
	quadPos[3].y = 50.0f;

	quadColorRGBY[0].r = 1.0f;
	quadColorRGBY[0].g = 0.0f;
	quadColorRGBY[0].b = 0.0f;
	quadColorRGBY[0].a = 1.0f;

	quadColorRGBY[1].r = 1.0f;
	quadColorRGBY[1].g = 1.0f;
	quadColorRGBY[1].b = 0.0f;
	quadColorRGBY[1].a = 1.0f;

	quadColorRGBY[2].r = 0.0f;
	quadColorRGBY[2].g = 1.0f;
	quadColorRGBY[2].b = 0.0f;
	quadColorRGBY[2].a = 1.0f;

	quadColorRGBY[3].r = 0.0f;
	quadColorRGBY[3].g = 0.0f;
	quadColorRGBY[3].b = 1.0f;
	quadColorRGBY[3].a = 1.0f;

	quadColorCYMW[0].r = 0.0f;
	quadColorCYMW[0].g = 1.0f;
	quadColorCYMW[0].b = 1.0f;
	quadColorCYMW[0].a = 1.0f;

	quadColorCYMW[1].r = 1.0f;
	quadColorCYMW[1].g = 1.0f;
	quadColorCYMW[1].b = 0.0f;
	quadColorCYMW[1].a = 1.0f;

	quadColorCYMW[2].r = 1.0f;
	quadColorCYMW[2].g = 0.0f;
	quadColorCYMW[2].b = 1.0f;
	quadColorCYMW[2].a = 1.0f;

	quadColorCYMW[3].r = 1.0f;
	quadColorCYMW[3].g = 1.0f;
	quadColorCYMW[3].b = 1.0f;
	quadColorCYMW[3].a = 1.0f;

	quadColorGray[0].r = 0.75f;
	quadColorGray[0].g = 0.75f;
	quadColorGray[0].b = 0.75f;
	quadColorGray[0].a = 1.0f;

	quadColorGray[1].r = 0.50f;
	quadColorGray[1].g = 0.50f;
	quadColorGray[1].b = 0.50f;
	quadColorGray[1].a = 0.50f;

	quadColorGray[2].r = 0.75f;
	quadColorGray[2].g = 0.75f;
	quadColorGray[2].b = 0.75f;
	quadColorGray[2].a = 1.0f;

	quadColorGray[3].r = 0.50f;
	quadColorGray[3].g = 0.50f;
	quadColorGray[3].b = 0.50f;
	quadColorGray[3].a = 1.0f;

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;

	glGenBuffers(1, &gVertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(LVertexPos2D), quadPos, GL_STATIC_DRAW);

	glGenBuffers(1, &gRGBYVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gRGBYVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(LColorRGBA), quadColorRGBY, GL_STATIC_DRAW);

	glGenBuffers(1, &gCYMWVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gCYMWVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(LColorRGBA), quadColorCYMW, GL_STATIC_DRAW);

	glGenBuffers(1, &gGrayVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gGrayVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(LColorRGBA), quadColorGray, GL_STATIC_DRAW);

	glGenBuffers(1, &gIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &gLeftVAO);
	glBindVertexArray(gLeftVAO);

	gMultiPolygon.enableDataPointers();

	glBindBuffer(GL_ARRAY_BUFFER, gVertexVBO);
	gMultiPolygon.setVertexPointer(0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, gRGBYVBO);
	gMultiPolygon.setColorPointer(0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, gGrayVBO);
	gMultiPolygon.setColor2Pointer(0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glBindVertexArray(0x0);

	glGenVertexArrays(1, &gRightVAO);
	glBindVertexArray(gRightVAO);

	gMultiPolygon.enableDataPointers();

	glBindBuffer(GL_ARRAY_BUFFER, gVertexVBO);
	gMultiPolygon.setVertexPointer(0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, gCYMWVBO);
	gMultiPolygon.setColorPointer(0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, gGrayVBO);
	gMultiPolygon.setColor2Pointer(0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glBindVertexArray(0x0);

	return true;
}

void update(void) {
}

void render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	renderScene();

	glutSwapBuffers();
}

void renderScene(void) {
	gMultiPolygon.setModelView(glm::translate<GLfloat>(glm::vec3(SCREEN_WIDTH * 1.0f / 4.0f, SCREEN_HEIGHT / 2.0f, 0.0f)));
	gMultiPolygon.updateModelView();

	glBindVertexArray(gLeftVAO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

	gMultiPolygon.setModelView(glm::translate<GLfloat>(glm::vec3(SCREEN_WIDTH * 3.0f / 4.0f, SCREEN_HEIGHT / 2.0f, 0.0f)));
	gMultiPolygon.updateModelView();

	glBindVertexArray(gRightVAO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);
}

void handleKeys(unsigned char key, int x, int y) {
//////////////////////////////////////////////////
//		GCC FILED SOME COMPLAINTS									//
//////////////////////////////////////////////////
	if (key == '?') {
		fprintf(stdout, "What is x/y? Baby dont %i, %i, me! NO mo'!\n", x, y);
	}
//////////////////////////////////////////////////
//		END OF GCC COMPLAINTS											//
//////////////////////////////////////////////////

	if (key == 'w') {
		gCameraY -= 16.0f;
	}

	if (key == 'a') {
		gCameraX -= 16.0f;
	}

	if (key == 's') {
		gCameraY += 16.0f;
	}

	if (key == 'd') {
		gCameraX += 16.0f;
	}
}

void handleMouseMotion(int x, int y) {
	if (x == 0) {
		if (y == 0) {
			fprintf(stdout, "x and y are 0");
		}
	}
}
