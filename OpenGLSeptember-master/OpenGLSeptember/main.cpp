#include <iostream>
#include <GL\glew.h>
#include "Display.h"
#include "Light.h"
#include "Shader.h"
#include "LightShader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Terrain.h"
#include "Camera.h"
#include "TimeKeeper.h"
#include "InputHandler.h"
#include "GBuffer.h"

#define NUM_POINT_LIGHTS 10
#define WIDTH 800
#define HEIGHT 600

/* Deferred rendering functions */
void LightPass(GBuffer* gbuf, GLuint quadVBO);
void GeometryPass(GBuffer* gbuf, std::vector<Mesh*> meshesToScene);
void SetupQuad(GLuint* quadVAO, GLuint* quadVBO);
void RenderQuad(GLuint quadVBO);


int main(int argc, char ** argv)
{
	TimeKeeper timer;
	Display display(WIDTH, HEIGHT, "3D-Project", &timer);

	
	GLuint quadVAO, quadVBO = 0;
	Shader geoShader("basicGeometryShader");
	LightShader lightShader("basicLightShader");
	SetupQuad(&quadVAO, &quadVBO);

	Texture texture("./textures/grass.jpg");
	Camera camera(glm::vec3(0, 0, -3), 70.f, (float)WIDTH / (float)HEIGHT, 0.01f, 1000.f);
	InputHandler inputHandler;
	Transform transform;

	Terrain terrain("heightmaps/heightmap1.bmp", 10);
	//Mesh mesh2("./objects/monkey.obj");
	std::vector<Mesh*> meshes;
	meshes.push_back(terrain.GetMesh());

	GBuffer gbuffer(WIDTH, HEIGHT);

	// Generate Lights //
	DirectionalLight directionalLight;
	PointLight pointLights;

	lightShader.InitLightUniforms();
	// -------------- //

	float counter = 0.f;
	while (!display.IsClosed())
	{
		display.Clear(1.f, 0.9f, 0.1f, 1.0f);
		//transform.GetRot().y = counter;

		geoShader.Bind();
		texture.Bind(0);
		GeometryPass(&gbuffer, meshes);

		lightShader.Bind();
		LightPass(&gbuffer, quadVBO);

		camera.Update();
		geoShader.Update(transform, camera);
		display.Update();

		counter += 0.01f;
	}
	return 0;
}

void GeometryPass(GBuffer* gbuf, std::vector<Mesh*> meshesToScene)
{
	gbuf->BindForWriting();
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	for (size_t i = 0; i < meshesToScene.size(); i++)
	{
		meshesToScene[i]->Draw();
	}

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);	
}

void LightPass(GBuffer* gbuf, GLuint quadVBO)
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gbuf->BindForReading();
	
	RenderQuad(quadVBO);
}


void SetupQuad(GLuint* quadVAO, GLuint* quadVBO) {
    GLfloat quadVertices[] = {
        // Positions        // Texture Coords
        -1.0f,  1.0f,    0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,    0.0f,  0.0f, 0.0f,
        1.0f,   1.0f,   0.0f,  1.0f, 1.0f,
        1.0f,  -1.0f,   0.0f,  1.0f, 0.0f,
    };
    glGenVertexArrays(1, quadVAO);
    glBindVertexArray(*quadVAO);

    glGenBuffers(1, quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, *quadVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);    //NTS: Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

    glEnableVertexAttribArray(1);    //NTS: Texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
}
void RenderQuad(GLuint quadVAO)
{
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}