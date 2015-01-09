#ifndef LSHADER_PROGRAM_H
#define LSHADER_PROGRAM_H

#include <string>
#include <fstream>

#include "../ogl/LOpenGL.hpp"

class LShaderProgram {
	public:
		LShaderProgram(void);
		virtual ~LShaderProgram(void);

		virtual bool loadProgram(void) = 0;
		virtual void freeProgram(void);

		bool bind(void);
		void unbind(void);
		GLuint getProgramID(void);

	protected:
		void printProgramLog(GLuint program);
		void printShaderLog(GLuint shader);
		GLuint loadShaderFromFile(std::string path, GLenum shaderType);

		GLuint mProgramID;
};

#endif
