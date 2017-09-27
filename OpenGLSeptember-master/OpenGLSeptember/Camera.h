#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <SDL2\SDL.h>

#include <iostream>

static const glm::vec3 XAxis(1, 0, 0);
static const glm::vec3 YAxis(0, 1, 0);
static const glm::vec3 ZAxis(0, 0, 1);

#define SENSITIVITY 0.1

class Camera
{
public:
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
	{
		this->m_perspective = glm::perspective(fov, aspect, zNear, zFar);
		this->m_pos = pos;
		this->m_forward = ZAxis;
		this->m_up = YAxis;

		this->m_right = glm::normalize(glm::cross(this->m_forward, this->m_up));

		this->m_oldMousePos = glm::vec2(0.0, 0.0);
	}

	void MoveForward(float amount = SENSITIVITY)
	{
		//std::cout << "m_forward: " << m_forward.x * amount << ", " << m_forward.y * amount<< ", " << m_forward.z * amount<< std::endl;
		this->m_pos += this->m_forward * amount;
	}

	void MoveRight(float amount = SENSITIVITY)
	{

		this->m_pos += this->m_right * amount;
	}

	void MoveUp(float amount = SENSITIVITY)
	{
		this->m_pos += this->m_up * amount;
	}
	
	void UpdateMouseView(const glm::vec2& newMousePos)
	{

	//	std::cout << "m_pos: " << m_pos.x << ", " << m_pos.y << ", " << m_pos.z << std::endl;
		glm::vec2 mouseDelta = newMousePos - this->m_oldMousePos;
		if (glm::length(mouseDelta) > 50.f)
		{
			this->m_oldMousePos = newMousePos;
			return;
		}
		const float ROTATIONAL_SPEED = SENSITIVITY*SENSITIVITY;
		this->m_right = glm::normalize(glm::cross(this->m_forward, this->m_up));
		glm::mat4 rotateMatrix = glm::mat3(glm::rotate(-mouseDelta.x * ROTATIONAL_SPEED, this->m_up)) *
								 glm::mat3(glm::rotate(-mouseDelta.y * ROTATIONAL_SPEED, this->m_right));

		this->m_forward = glm::normalize( glm::mat3(rotateMatrix) * this->m_forward);

		this->m_oldMousePos = newMousePos;
	}

	void Update()
	{
		SDL_Delay(10); // experoiment with this!
		while (SDL_PollEvent(&this->m_event)) 
		{

			switch(this->m_event.type) 
			{
			case SDL_KEYDOWN:
				//std::cout << "inside Update()" << std::endl;
				switch (this->m_event.key.keysym.sym)
				{
				case SDLK_w:
					this->MoveForward();
					break;
				case SDLK_a:
					this->MoveRight(-SENSITIVITY);
					break;
				case SDLK_s:
					this->MoveForward(-SENSITIVITY);
					break;
				case SDLK_d:
					this->MoveRight();
					break;
				case SDLK_SPACE:
					this->MoveUp();
					break;
				case SDLK_x:
					this->MoveUp(-SENSITIVITY);
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				this->UpdateMouseView(glm::vec2(this->m_event.motion.x, this->m_event.motion.y));
				break;
			default:
				break;
			}
		}

	}
		

	inline glm::vec3 GetPos() const { return this->m_pos; };


	inline glm::mat4 GetViewProjection() const
	{
		return this->m_perspective * glm::lookAt(this->m_pos, this->m_pos + this->m_forward, this->m_up);
	}

	~Camera(){}

private:
	glm::mat4 m_perspective;
	glm::vec3 m_pos;
	glm::vec3 m_forward;
	glm::vec3 m_up;
	glm::vec3 m_right;


	glm::vec2 m_oldMousePos;

	SDL_Event m_event;
};

