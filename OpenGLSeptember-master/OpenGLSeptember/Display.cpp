#include "Display.h"
#include <GL\glew.h>
#include <iostream>


Display::Display(int width, int height, const std::string& title, TimeKeeper* timer, int fps)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	// Gives OpenGL one buffer to write to and the window another to display
													// Avoid displaying something drawing in progress.
	this->m_width = width;
	this->m_height = height;
	this->m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	this->m_glContext = SDL_GL_CreateContext(this->m_window);
	this->m_fps = fps;
	this->m_screenTicksPerFrame = 1000 / this->m_fps;
	this->m_frames = 0;


	this->m_timerPtr = timer;

	GLenum status = glewInit();

	if (status != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize!" << std::endl;
	}

	m_isClosed = false;

	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
}


void Display::Update()
{
	SDL_GL_SwapWindow(this->m_window);
	this->m_timerPtr->SetTimer();

	SDL_WarpMouseInWindow(this->m_window, this->m_width / 2,  this->m_height/ 2);

	int timeElasped = m_timerPtr->GetTimeElapsed();

	if (timeElasped < this->m_screenTicksPerFrame)
	{
		SDL_Delay(this->m_screenTicksPerFrame - timeElasped);
	}

	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
		{
			this->m_isClosed = true;
		}
	}
}

void Display::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
bool Display::IsClosed()
{
	return this->m_isClosed;
}

Display::~Display()
{
	SDL_GL_DeleteContext(this->m_glContext);
	SDL_DestroyWindow(this->m_window);
	SDL_Quit();
}
