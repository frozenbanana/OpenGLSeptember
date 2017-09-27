#pragma once

#include <GL\glew.h>

class GBuffer
{
public:

	 enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_DIFFUSE,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_TEXTURE_TYPE_TEXCOORD,
        GBUFFER_NUM_TEXTURES
    };

	GBuffer(unsigned int WindowWidth, unsigned int WindowHeight);
	~GBuffer();

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

    void BindForWriting();

    void BindForReading();

	void SetReadBuffer(GBUFFER_TEXTURE_TYPE textureType);
private:
	GLuint m_frameBufferObject;
	GLuint m_textures[GBUFFER_NUM_TEXTURES];
	GLuint m_depthTexture;

};

