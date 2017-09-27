#pragma once
#include <string>
#include <GL\glew.h>
#include "Transform.h"
#include "Camera.h"

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

class Shader
{
public:
	Shader(const std::string& fileName);

	void Bind();
	void Update(const Transform& transform, const Camera& camera);
	virtual ~Shader();

protected:
	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(GLuint shader, GLint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string& text, unsigned int type);
	enum {
		VERTEX_S,
		//GEOMETRY_S,
		FRAGMENT_S,

		NUM_SHADER
	};

	enum {
		WORLD_U,
		VIEWPROJECTION_U,
		CAMERA_POS_U,
		NUM_UNIFORMS
	};

	GLuint m_program;
	GLuint m_shaders[NUM_SHADER];
	GLuint m_uniforms[NUM_UNIFORMS];

};

