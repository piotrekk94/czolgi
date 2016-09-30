#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <sstream>
#include <fstream>
#include <vector>

class ShaderProgram
{
	public:
		GLuint getUniformLocation(const char *name);
		GLuint getAttribLocation(const char *name);
		GLuint getUniformBlockIndex(const char *name);
		GLuint getUniformLocation(const std::string name);
		ShaderProgram &use();
		GLuint getID(){return programID;};

		void setVector3f(const GLchar *name, const glm::vec3 &value);
    		void setVector4f(const GLchar *name, const glm::vec4 &value);

		ShaderProgram(const char *vertex_file_path,const char *fragment_file_path); // można to poprawić
		~ShaderProgram();
	private:
		GLuint programID;
		GLuint vertexShaderID;
		GLuint fragmentShaderID;
		std::string readFile(const char *fileName);
};
