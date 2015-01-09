#ifndef LMULTI_COLOR_POLYGON_PROGRAM_2D
#define LMULTI_COLOR_POLYGON_PROGRAM_2D

#include <glm/glm.hpp>
#include "../shader/LShaderProgram.hpp"

class LMultiColorPolygonProgram2D : public LShaderProgram {
	public:
		LMultiColorPolygonProgram2D(void);

		bool loadProgram(void);

		void setVertexPointer(GLsizei stride, const GLvoid *data);
		void setColorPointer(GLsizei stride, const GLvoid *data);

		void enableVertexPointer(void);
		void disableVertexPointer(void);

		void enableColorPointer(void);
		void disableColorPointer(void);

		void setProjection(glm::mat4 matrix);
		void setModelView(glm::mat4 matrix);

		void leftMultProjection(glm::mat4 matrix);
		void leftMultModelView(glm::mat4 matrix);

		void updateProjection(void);
		void updateModelView(void);

	private:
		GLint mVertexPos2DLocation;
		GLint mMultiColorLocation;

		GLint mProjectionMatrixLocation;
		GLint mModelViewMatrixLocation;

		glm::mat4 mProjectionMatrix;
		glm::mat4 mModelViewMatrix;
};

#endif
