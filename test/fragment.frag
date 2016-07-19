#version 330 core
in vec4 vertex2;
in vec2 UV;
in vec4 normals;
 float d;
 vec4 toLight;
 vec4 toViewer;
out vec4 color;
float ambient = 0.2;

uniform vec4 lightPosition[5];
uniform vec3 lightColor[5];
float lightPower[5];

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
	vec4 actualLightPosition;
	actualLightPosition = V * lightPosition[0];
	d = length(vertex2 - actualLightPosition);
	toLight = normalize(V * lightPosition[0] - V * M * vertex2);
	toViewer = normalize(vec4(0,0,0,1) - V * M * vertex2);
	lightPower[0] = 200;
	/////////////////////////////
	float Il = lambert(actualLightPosition, lightColor[0], lightPower[0]);
	float Ip = phong(actualLightPosition, lightColor[0], lightPower[0], 100);
	vec4 textureColor = vec4(1,1,0,0);
	if (hasTexture == 1) textureColor = texture( myTexture, UV);
	color.rgb = textureColor.rgb * ambient + simpleShading(textureColor.rgb, lightColor[0] * Il , d*d);
//	color.rgb += simpleShading(textureColor.rgb , lightColor[0] * Ip , d * d); //ostre jazdy
//	color.rgb = lightColor[0] * Il;
}
float phong(vec4 lightPosition, vec3 lightColor , float power, float brightness)
{
	vec4 norm = normalize(normals);
	vec4 P = normalize(-toLight);//???
//	P = reflect(P, norm);
	vec4 V = normalize(toViewer);
	float I = pow(clamp(dot(P, V), 0.0, 1.0), brightness);
	return I * power;

}
float lambert(vec4 lightPosition, vec3 lightColor, float power)
{
	vec4 norm = normalize(normals);
	vec4 L = normalize(toLight);
	float I = dot(norm, L); //???
	return clamp(I, 0.0f, 1.0f) * power;
}
vec3 simpleShading(vec3 material, vec3 light, float distance)
{
	return material * light / distance;
}
