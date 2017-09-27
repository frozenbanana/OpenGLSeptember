#version 420
out vec4 fragColor1;

in vec2 texCoord0;

uniform sampler2D gPositionMap;
uniform sampler2D gDiffuseMap;
uniform sampler2D gNormalMap;
uniform sampler2D gTexCoordMap;

struct BaseLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct DirectionalLight
{
    BaseLight Base;
    vec3 Direction;
};

struct Attenuation
{
    float Constant;
    float Linear;
    float Exp;
};

struct PointLight
{
    BaseLight Base;
    vec3 Position;
    Attenuation Atten;
};

uniform vec3 directionalLightVec;
uniform vec3 camPos;

uniform PointLight gPointLight;
uniform float gScreenSize;

vec2 CalcTexCoord();
//float CalcDirectionalLight(vec3 worldPos, vec3 normal);
float CalcPointLight(vec3 worldPos, vec3 normal);
float CalcLightInternal(BaseLight light, vec3 worldPos, vec3 normal);

void main()
{
	vec2 texCoord1 = CalcTexCoord();
	vec3 worldPos1 = texture(gPositionMap, texCoord0).rgb;
	vec3 diffuse1 = texture(gDiffuseMap, texCoord0).rgb;
	vec3 normal1 = texture(gNormalMap, texCoord0)
	
	fragColor1 = CalcPointLight(worldPos1, normal1);
}	

vec4 CalcPointLight(worldPos1, vec3 normal1)
{
	vec3 LightDirection = WorldPos - gPointLight.Position;
	float Distance = length(LightDirection);
	LightDirection = normalize(LightDirection);

	vec4 Color = CalcLightInternal(gPointLight.Base, LightDirection, WorldPos, Normal);

	float Attenuation =  gPointLight.Atten.Constant +
						 gPointLight.Atten.Linear * Distance +
						 gPointLight.Atten.Quadratic * Distance * Distance;

	Attenuation = max(1.0, Attenuation);

	return Color / Attenuation;
};

vec4 CalcLightInternal(BaseLight light, vec3 lightDirection, vec3 worlPos, vec3 normal)
{
	ve4 ambientColor = vec4(light.Color, 1.0f) * light.AmbientIntensity;
	float diffuseFactor = dot(normal, -lightDirection);

	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);

	if (diffuseColor > 0)
	{
		diffuseColor = vec4(light.Color, 1.0f) * light.DiffuseIntensity * diffuseFactor;

		vec3 vertexToCam = normalize(camPos - worldPos);
		vec3 lightReflect = normalize(refelect(lightDirection, normal));
		float specularFactor = max(0.0, dot(vertexToEye lightReflect));
		if(specularFactor > 0 )
		{
			specularColor = vec4(light.Color, 1.0f)  * specularFactor * 0.3f; // * materialShininess
		}
	}
	
	return ambientColor + diffuseColor + specularColor;
}
vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / gScreenSize;
}

