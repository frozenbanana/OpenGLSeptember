#include "Shader.h"
#include <iostream>
#include <fstream>


Shader::Shader(const std::string& fileName)
{
	this->m_program = glCreateProgram();
	this->m_shaders[VERTEX_S] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	//this->m_shaders[GEOMETRY_S] = CreateShader(LoadShader(fileName + ".gs"), GL_GEOMETRY_SHADER);
	this->m_shaders[FRAGMENT_S] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for (size_t i = 0; i < this->NUM_SHADER; i++)
	{
		glAttachShader(this->m_program, this->m_shaders[i]);
	}

	glBindAttribLocation(this->m_program, 0, "position");
	glBindAttribLocation(this->m_program, 1, "texCoord");
	glBindAttribLocation(this->m_program, 2, "normal");

	glLinkProgram(this->m_program);
	this->CheckShaderError(this->m_program, GL_LINK_STATUS, true ,"Error linking shader program:");

	glValidateProgram(this->m_program);
	CheckShaderError(this->m_program, GL_LINK_STATUS, true, "Invalid shader program:");

	this->m_uniforms[WORLD_U] = glGetUniformLocation(this->m_program, "world");
	this->m_uniforms[VIEWPROJECTION_U] = glGetUniformLocation(this->m_program, "viewProjection");
}

void Shader::Bind()
{
	glUseProgram(this->m_program);
}

void Shader::Update(const Transform& transform, const Camera& camera)
{
	glm::mat4 world = transform.GetModelMatrix();
	glm::mat4 viewProjection = camera.GetViewProjection();
	glm::vec3 camPos = camera.GetPos();
	glUniformMatrix4fv(this->m_uniforms[WORLD_U], 1, GL_FALSE, &world[0][0]);
	glUniformMatrix4fv(this->m_uniforms[VIEWPROJECTION_U], 1, GL_FALSE, &viewProjection[0][0]);
	glUniform3fv(this->m_uniforms[CAMERA_POS_U], 1, &camPos[0]);
}



Shader::~Shader()
{
	for (size_t i = 0; i < this->NUM_SHADER; i++)
	{
		glDetachShader(this->m_program, this->m_shaders[i]);
		glDeleteShader(this->m_shaders[i]);
	}

	glDeleteProgram(this->m_program);
}



std::string Shader::LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());
	
	std::string output;
	std::string line;
	
	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void Shader::CheckShaderError(GLuint shader, GLint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
	{
		glGetProgramiv(shader, flag, &success);
	}
	else
	{
		glGetShaderiv(shader, flag, &success);
	}

	if (success == GL_FALSE)
	{
		if (isProgram)
		{
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		}
		else
		{
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}

		std::cerr << errorMessage << ": " << error << "'" << std::endl;
	}
}

GLuint Shader::CreateShader(const std::string& text, unsigned int type){

	GLuint shader = glCreateShader(type);

	if (shader == 0)
	{
		std::cerr << "Error compiling shader type " << type << std::endl;
	}

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLengths[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	return shader;
}