#pragma once
#include <SDL2\SDL.h>
#include <GL\glew.h>
#include <string>
#include <iostream>
#include "TimeKeeper.h"


class Display
{
public:
	Display(int width, int height, const std::string& title, TimeKeeper* timer, int fps = 60);
	void Update();
	bool IsClosed();
	void Clear(float r, float g, float b, float a);
	~Display();
private:
	SDL_Window* m_window;
	SDL_GLContext m_glContext;		// Gives GPU control to window from operating system
	int m_width, m_height;
	bool m_isClosed;
	int m_fps;
	int m_frames;
	int m_screenTicksPerFrame;
	TimeKeeper* m_timerPtr;

};

