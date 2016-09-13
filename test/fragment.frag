#version 330 core
in mat4 TBN; //macierz przekształcająca z camera space na tangent space
out vec4 outputColor;
in vec4 vertex_camera;
in vec2 UV;
in vec4 normals;
uniform vec4 color;
uniform vec4 specularColor;
uniform float shinniness;
uniform float ambient;
//
uniform int lightNumber;
struct Light
{
	vec4 position;//camera space
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
uniform mat4 ITMV;//odwrócona transponowana macierz V * M
uniform sampler2D textureArray[2];
uniform int hasTexture;
uniform int hasBump;
const int hasBump2 = 1;
vec4 textureColor = color;
vec4 useSpecularColor;

//functions
float phong(vec4 toLight, vec4 toViewer, float power, float shinniness);
vec3 simpleShading(vec3 material, vec3 light, float distance);
float lambert(vec4 toLight, float power);
vec3 calcLight();
vec3 gamma = vec3(2.2);
vec4 norm = normalize(normals);

void main()
{
	/////////////////////////////
	if (hasTexture == 1)
	 {
		 textureColor = texture( textureArray[0], UV);
	 }

	if (hasBump == 1)
	 {
		norm = normalize(vec4(texture(textureArray[1], UV).rgb * 2.0 - 1.0,0));
	 }
	textureColor.rgb = pow(textureColor.rgb, gamma);//gamma correction
	useSpecularColor.rgb = pow(specularColor.rgb, gamma);//gamma correction
	outputColor.rgb = calcLight();
	outputColor.a = textureColor.a;
//	outputColor.rgb = light[0].color * Il;
}

vec3 calcLight()
{
	float d;
	vec4 toLight;
	vec4 toViewer;
	vec3 color = textureColor.rgb * ambient;
	for (int i = 0; i < lightNumber ; i++)
	{
		switch (light[i].type )
		{
			case 1:
				{
					d = length((vertex_camera - light[i].position));//optymalizacje potrzebne
					toLight = normalize(light[i].position - vertex_camera);
					break;
				}
			case 2:
				{
					d = 0;
					toLight = normalize(light[i].position);
					break;
				}
			default:
				continue;
				break;
		}
		toViewer = normalize((vec4(0,0,0,1) - vertex_camera));
		if (hasBump == 1)
		{
			toLight = normalize(TBN * toLight);
			toViewer = normalize(TBN * toViewer);
		}
		float Il = lambert(toLight, light[i].power);
		float Ip = phong(toLight, toViewer, light[i].power, shinniness);
		color.rgb += simpleShading(textureColor.rgb, light[i].color * Il , d);
		//		color.rgb += simpleShading(useSpecularColor.rgb , light[i].color * Ip , d);
	}
	return pow(color, 1/gamma);//gamma correction

}

float phong(vec4 toLight, vec4 toViewer, float power, float shinniness)
{
	vec4 P = -normalize(toLight);//???
	P = normalize( reflect(P, norm));
	vec4 V = normalize(toViewer);
	float I = pow(clamp(dot(P, V) * power, 0.0f, 1.0f), shinniness);
	return I;

}

float lambert(vec4 toLight, float power)
{
	vec4 L = normalize(toLight);
	float I = dot(norm, L); //???
	return clamp(I * power, 0.0f, 1.0f);
}

vec3 simpleShading(vec3 material, vec3 light, float distance)
{
	return material * light / (1 + distance * distance);
}
