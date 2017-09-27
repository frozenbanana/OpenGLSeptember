#version 420

uniform sampler2D diffuse;

in vertexData
{
	vec3 normal;
	vec2 texCoord;
} f_inData;

void main()
{
	gl_FragColor = texture2D(diffuse, f_inData.texCoord) * clamp(dot(-vec3(0,0,1), f_inData.normal), 0.0, 1.0);
	
}