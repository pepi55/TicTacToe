#ifndef LMULTI_POLYGON_PROGRAM_H
#define LMULTI_POLYGON_PROGRAM_H

#include <glm/glm.hpp>
#include "../shader/LShaderProgram.hpp"

class LMultiPolygonProgram2D : public LShaderProgram {
	public:
		LMultiPolygonProgram2D(void);

		void setVertexPointer(GLsizei stride, const GLvoid *data);
		void setColorPointer(GLsizei stride, const GLvoid *data);
		void setColor2Pointer(GLsizei stride, const GLvoid *data);

		void enableDataPointers(void);
		void disableDataPointers(void);

		bool loadProgram(void);

		void setProjection(glm::mat4 matrix);
		void setModelView(glm::mat4 matrix);

		void leftMultProjection(glm::mat4 matrix);
		void leftMultModelView(glm::mat4 matrix);

		void updateProjection(void);
		void updateModelView(void);

	private:
		GLint mVertexPos2DLocation;
		GLint mMultiColorLocation1;
		GLint mMultiColorLocation2;

		GLint mProjectionMatrixLocation;
		GLint mModelViewMatrixLocation;

		glm::mat4 mProjectionMatrix;
		glm::mat4 mModelViewMatrix;
};

#endif
