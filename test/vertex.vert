#version 330 core
in vec3 vertex;
in vec2 vertexTexture;
out vec2 UV;
in vec3 Normals;
out vec4 normals;
out vec4 vertex2;
out float d;
out vec4 toLight;
out vec4 toViewer;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 MVP;
uniform mat4 ITMV;

void main()
{
	vertex2 = vec4(vertex, 1);
	gl_Position = MVP * vertex2;
	UV = vec2(vertexTexture.x, 1.0 - vertexTexture.y);
	normals = ITMV* vec4(Normals , 0) ;
}
