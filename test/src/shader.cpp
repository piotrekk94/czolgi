#include "shader.hpp"
#define DEBUG 1
GLuint ShaderProgram::getUniformBlockIndex(const char *name)
{
	return glGetUniformBlockIndex(programID, name);
}
GLuint ShaderProgram::getUniformLocation(const char *name)
{
	return glGetUniformLocation(programID, name);
}
GLuint ShaderProgram::getUniformLocation(const std::string name)
{
	return glGetUniformLocation(programID, name.c_str());
}
GLuint ShaderProgram::getAttribLocation(const char *name)
{
	return glGetAttribLocation(programID, name);
}
void ShaderProgram::on()
{
	glUseProgram(programID);
}
ShaderProgram::ShaderProgram(const char *vertex_file_path,const char *fragment_file_path) {

	//   Create the shaders
	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;
	// Read vertex code
	std::string VertexShaderCode = readFile(vertex_file_path);
	// Read fragment code
	std::string FragmentShaderCode = readFile(fragment_file_path);

	// Compile Vertex Shader
	if (DEBUG) printf("Compiling shader : %s\n", vertex_file_path);
	char const *VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	if (DEBUG) printf("Compiling shader: %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	//Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	programID = glCreateProgram();
	glAttachShader(programID, VertexShaderID);
	glAttachShader(programID, FragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
}
ShaderProgram::~ShaderProgram()
{
	glDetachShader(programID, VertexShaderID);
	glDetachShader(programID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	glDeleteProgram(programID);
}

std::string ShaderProgram::readFile(const char *fileName)
{
	std::stringstream text;
	std::ifstream file(fileName, std::ios::in);
	if (file.is_open())
	{
		text << file.rdbuf();
	}
	else
	{
		fprintf(stderr,"Error file: %s.", fileName);
		return std::string("");
	}
	return text.str();
}
