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
uniform vec4 lightPosition[5];
mat4 mvp;

void main()
{
	mvp = P * V * M;
	vertex2 = vec4(vertex, 1);
	gl_Position = P * V * M * vertex2;
	UV = vec2(vertexTexture.x, 1.0 - vertexTexture.y);
	normals = normalize(V * M * vec4( Normals, 1)) ;
	vec4 actualLightPosition = V * lightPosition[0];
}
