#version 330 core
in vec4 vertex2;
in vec2 UV;
out vec4 color;
uniform sampler2D myTexture;
void main()
{

//	vec3 vertex3 = vertex2.xyz - vec3(2,2,2);
//	float d = sqrt(vertex3.x*vertex3.x + vertex3.y*vertex3.y + vertex3.z*vertex3.z)/9;
//	color = vec3(1 - d,0,0);
	color = texture( myTexture, UV);
}

