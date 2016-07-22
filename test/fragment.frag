#version 330 core
in vec4 vertex2;
in vec2 UV;
in vec4 normals;
out vec4 color;
float ambient = 0.2;
// 
uniform int lightNumber;
struct Light
{
vec4 position;
vec3 color;
float power;
int type;
};
//layout (std140) uniform LightBlock
//{
uniform	Light light[3];
//};
//
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform sampler2D myTexture;
uniform int hasTexture;
vec4 textureColor = vec4(1,1,0,0);
//functions
float phong(vec4 toLight, vec4 toViewer, float power, float shinniness);
vec3 simpleShading(vec3 material, vec3 light, float distance);
float lambert(vec4 toLight, float power);
vec3 calcLight();
//
vec4 norm = normalize(V * M *  normals);
void main()
{
	/////////////////////////////
	if (hasTexture == 1) textureColor = texture( myTexture, UV); 
	color.rgb = calcLight();
//	color.rgb = light[0].color * Il;
}
vec3 calcLight()
{
	float d;
	vec4 toLight;
	vec4 toViewer;
	vec3 color = textureColor.rgb * ambient;
	vec4 actualLightPosition;
	for (int i = 0; i < lightNumber ; i++)
	{
		switch (light[i].type ) 
		{
			case 1:
				{
					actualLightPosition = V * light[i].position;
					d = length(vertex2 - actualLightPosition);
					toLight = normalize(V * light[i].position - V * M * vertex2);
					break;
				}
			case 2:
				{
					d = 1;
					toLight = V * light[i].position;
					break;
				}
			default:
				continue;
				break;
		}
		toViewer = normalize(vec4(0,0,0,1) - V * M * vertex2);
		float Il = lambert(toLight, light[i].power);
		float Ip = phong(toLight, toViewer, light[i].power, 100);
		color.rgb += simpleShading(textureColor.rgb, light[i].color * Il , d*d);
		color.rgb += simpleShading(textureColor.rgb , light[i].color * Ip , d * d);
	}
	return color;

}
float phong(vec4 toLight, vec4 toViewer, float power, float shinniness)
{
	vec4 P = normalize(-toLight);//???
	P = reflect(P, norm);
	vec4 V = normalize(toViewer);
	float I = pow(clamp(dot(P, V), 0.0f, 1.0f), shinniness);
	return I * power;

}
float lambert(vec4 toLight, float power)
{
	vec4 L = normalize(toLight);
	float I = dot(norm, L); //???
	return clamp(I, 0.0f, 1.0f) * power;
}
vec3 simpleShading(vec3 material, vec3 light, float distance)
{
	return material * light / distance;
}
