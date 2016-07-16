#version 330 core
in vec4 vertex2;
in vec2 UV;
in vec4 normals;
out vec4 color;
float ambient = 0.2;
vec4 lightPosition0;
vec3 lightColor0;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform sampler2D myTexture;
vec3 simpleShading(vec3 material, vec3 light, float distance);
void main()
{
	lightPosition0 = vec4(0,0,0,1);
	lightPosition0 = P * V * lightPosition0;
	lightColor0 = vec3(5,5,5);
	vec4 L = lightPosition0 - vertex2;
	L = normalize(L);
	L.w = 1;
	float I = dot(L, normals); //???
	vec3 vertex3 = vertex2.xyz - lightPosition0.xyz;
	float d = sqrt(vertex3.x*vertex3.x + vertex3.y*vertex3.y + vertex3.z*vertex3.z);
	color = texture( myTexture, UV);
	color.rgb = simpleShading(color.rgb, lightColor0 , d);
}
vec3 simpleShading(vec3 material, vec3 light, float distance)
{
	vec3 result;
	result.x = ambient * material.x + light.x * material.x / ( distance * distance);
	result.y = ambient * material.y + light.y * material.y / ( distance * distance);
	result.z = ambient * material.z + light.z * material.z / ( distance * distance);
	return result;
}
