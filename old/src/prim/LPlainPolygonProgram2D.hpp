#ifndef LPLAIN_POLYGON_PROGRAM_2D_H
#define LPLAIN_POLYGON_PROGRAM_2D_H

#include <glm/glm.hpp>
#include "../shader/LShaderProgram.hpp"

class LPlainPolygonProgram2D : public LShaderProgram {
	public:
		LPlainPolygonProgram2D(void);

		bool loadProgram(void);

		void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);
		void setProjection(glm::mat4 matrix);
		void setModelView(glm::mat4 matrix);
		void leftMultProjection(glm::mat4 matrix);
		void leftMultModelView(glm::mat4 matrix);

		void updateProjection(void);
		void updateModelView(void);

	private:
		GLint mPolygonColorLocation;

		GLint mProjectionMatrixLocation;
		GLint mModelViewMatrixLocation;

		glm::mat4 mProjectionMatrix;
		glm::mat4 mModelViewMatrix;
};

#endif
