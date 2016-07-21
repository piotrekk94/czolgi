#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <sstream>
#include <fstream>
#include <vector>

class ShaderProgram
{
	public:
		GLuint getUniformLocation(const char *name);
		GLuint getAttribLocation(const char *name);
		void on();

		ShaderProgram(const char *vertex_file_path,const char *fragment_file_path); // można to poprawić
		~ShaderProgram();
	private:
		GLuint programID;
		GLuint VertexShaderID;
		GLuint FragmentShaderID;
		std::string readFile(const char *fileName);
};
