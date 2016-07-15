#version 330 core
in vec3 vertex;
in vec2 vertexTexture;
out vec2 UV;
out vec4 vertex2;
uniform mat4 mvp;

void main()
{
	gl_Position.xyz = vertex;
	gl_Position.w = 1.0f;
	gl_Position = mvp * gl_Position;
	vertex2 = gl_Position;
	UV = vec2(vertexTexture.x, 1.0 - vertexTexture.y);
}

