#version 330 core
in vec4 vertex2;
in vec2 UV;
in vec4 normals;
out vec4 color;
float ambient = 0.2;

uniform vec4 lightPosition0;
uniform vec3 lightColor0;
float lightPower0;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform sampler2D myTexture;
uniform int hasTexture;
//functions
float phong(vec4 lightPosition, vec3 lightColor , float power, float brightness);
vec3 simpleShading(vec3 material, vec3 light, float distance);
float lambert(vec4 lightPosition, vec3 lightColor, float power);
//
void main()
{
//	lightColor0 ;
	lightPower0 = 20;
	/////////////////////////////
//	lightPosition0 = vec4(0,0,0,1);
	vec4 lightPosition;
//	lightPosition = vec4(0,0,0,1);
	lightPosition = P * V * lightPosition0;
	float Il = lambert(lightPosition, lightColor0, lightPower0);
	float Ip = phong(lightPosition, lightColor0, lightPower0, 100);
	vec3 vertex3 = vertex2.xyz - lightPosition.xyz;
	float d = sqrt(vertex3.x*vertex3.x + vertex3.y*vertex3.y + vertex3.z*vertex3.z);
	vec4 textureColor = vec4(1,1,0,0);
	if (hasTexture == 1) textureColor = texture( myTexture, UV);
	color.rgb = textureColor.rgb * ambient + simpleShading(textureColor.rgb, lightColor0 * Il , d*d);
	color.rgb += simpleShading(textureColor.rgb , lightColor0 * Ip , d * d);
}
float phong(vec4 lightPosition, vec3 lightColor , float power, float brightness)
{
	vec4 P = lightPosition - vertex2;//???
	P = normalize(P);
	P.w = 1;
	P = reflect(P, normals);
	vec4 V = vec4(0,0,0,1) - vertex2;//??? camera chyba jest w 0,0,0
	V = normalize(V);
	float I = pow(clamp(dot(P, V),0,1), brightness);
	return clamp(I,0,1) * power;

}
float lambert(vec4 lightPosition, vec3 lightColor, float power)
{
	vec4 L = lightPosition - vertex2;
	L = normalize(L);
	L.w = 1;
	float I = dot(L, normals); //???
	return clamp(I,0,1) * power;
}
vec3 simpleShading(vec3 material, vec3 light, float distance)
{
	vec3 result;
	result.x = light.x * material.x / ( distance);
	result.x = clamp(result.x,0,1);
	result.y = light.y * material.y / ( distance);
	result.y = clamp(result.y,0,1);
	result.z = light.z * material.z / ( distance);
	result.z = clamp(result.z,0,1);
	return result;
}
