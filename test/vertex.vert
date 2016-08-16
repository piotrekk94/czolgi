#version 330 core
in vec3 vertex;
in vec3 bitangents;
in vec3 tangents;
in vec2 vertexTexture;
out vec2 UV;
in vec3 Normals;
out mat4 TBN;
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
uniform int hasBump;

void main()
{
	TBN = transpose(mat4(
	ITMV * normalize(vec4(tangents,0)),
	ITMV * normalize(vec4(bitangents,0)),
	ITMV * normalize(vec4(Normals,0)),
	vec4(0,0,0,1)));// ??? nadzieja umiera ostatnia
	vertex2 = vec4(vertex, 1);
	gl_Position = MVP * vertex2;
	UV = vec2(vertexTexture.x, vertexTexture.y);
	if (hasBump == 0)
	{
	normals = ITMV* vec4(Normals , 0);
	}

}
