#version 420

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

varying vec2 texCoord0;
varying vec3 normal0;

uniform mat4 world;
uniform mat4 viewProjection;

out vertexData
{
	vec3 normal;
	vec2 texCoord;
} v_outData;

void main()
{
	mat4 transform = viewProjection * world;
	gl_Position = transform * vec4(position, 1.0);
	v_outData.texCoord = texCoord;
	v_outData.normal = (transform * vec4(normal, 0.0)).xyz;
}