#ifndef LTEXTURED_POLYGON_2D_H
#define LTEXTURED_POLYGON_2D_H

#include <glm/glm.hpp>

#include "../shader/LShaderProgram.hpp"
#include "../shader/LColorRGBA.hpp"

class LTexturedPolygon2D : public LShaderProgram {
	public:
		LTexturedPolygon2D(void);

		bool loadProgram(void);

		void setVertexPointer(GLsizei stride, const GLvoid *data);
		void setTexCoordPointer(GLsizei stride, const GLvoid *data);

		void enableVertexPointer(void);
		void disableVertexPointer(void);

		void enableTexCoordPointer(void);
		void disableTexCoordPointer(void);

		void setProjection(glm::mat4 matrix);
		void setModelView(glm::mat4 matrix);

		void leftMultProjection(glm::mat4 matrix);
		void leftMultModelView(glm::mat4 matrix);

		void updateProjection(void);
		void updateModelView(void);

		void setTextureColor(LColorRGBA color);
		void setTextureUnit(GLuint unit);

	private:
		GLint mVertexPos2DLocation;
		GLint mTexCoordLocation;

		GLint mTextureColorLocation;
		GLint mTextureUnitLocation;

		glm::mat4 mProjectionMatrix;
		glm::mat4 mModelViewMatrix;

		GLint mProjectionMatrixLocation;
		GLint mModelViewMatrixLocation;
};

#endif
