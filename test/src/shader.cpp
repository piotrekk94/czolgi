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
ShaderProgram &ShaderProgram::use()
{
	glUseProgram(programID);
	return *this;
}
ShaderProgram::ShaderProgram(const char *vertex_file_path,const char *fragment_file_path) {

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint result = GL_FALSE;
	int infoLogLength;
	std::string vertexShaderCode = readFile(vertex_file_path);
	std::string fragmentShaderCode = readFile(fragment_file_path);

	if (DEBUG) printf("Kompilowanie vertex shader : %s\n", vertex_file_path);
	char const * codePtr = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &codePtr , NULL);
	glCompileShader(vertexShaderID);

	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}

	if (DEBUG) printf("Kompilowanie Fragment shader: %s\n", fragment_file_path);
	codePtr = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &codePtr , NULL);
	glCompileShader(fragmentShaderID);

		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
	}

	printf("Linkowanie\n");
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
	}
}
ShaderProgram::~ShaderProgram()
{
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
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
		fprintf(stderr,"Błąd, brak pliku: %s.", fileName);
		return std::string("");
	}
	return text.str();
}

void ShaderProgram::setVector3f(const GLchar *name, const glm::vec3 &value)
{
	glUniform3f(glGetUniformLocation(this->programID, name), value.x, value.y, value.z);
}
void ShaderProgram::setVector4f(const GLchar *name, const glm::vec4 &value)
{
	glUniform4f(glGetUniformLocation(this->programID, name), value.x, value.y, value.z, value.w);
}
