#version 330 core
in vec3 vertex;
in vec2 vertexTexture;
out vec2 UV;
in vec3 Normals;
out vec4 normals;
out vec4 vertex2;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
mat4 mvp;

void main()
{
	mvp = P * V * M;
	gl_Position.xyz = vertex;
	gl_Position.w = 1.0f;
	gl_Position = mvp * gl_Position;
	vertex2 = gl_Position;
	UV = vec2(vertexTexture.x, 1.0 - vertexTexture.y);
	normals = vec4(Normals, 1);
}

