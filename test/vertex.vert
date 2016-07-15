#version 330 core
in vec4 vertex;
out vec4 vertex2;
uniform mat4 mvp;
void main()
{
	gl_Position = vertex;
	gl_Position = mvp * gl_Position;
	vertex2 = vertex;
}

