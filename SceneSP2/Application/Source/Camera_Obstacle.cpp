#include "Camera_Obstacle.h"
#include "Application.h"
#include "Mtx44.h"
#include <iostream>

Camera_Obstacle::Camera_Obstacle()
{
}

Camera_Obstacle::~Camera_Obstacle()
{
}

void Camera_Obstacle::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera_Obstacle::Update(double dt, float jumpHeight, bool retry)
{
	static const float CAMERA_SPEED = 80;
	static const float ZOOM_SPEED = 80;

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	position.y = 10 + jumpHeight;

	if(Application::IsKeyPressed('A'))
	{
		
		position -= right * ZOOM_SPEED * dt;
		target = position + view;
	}
	if(Application::IsKeyPressed('D'))
	{
		position += right * ZOOM_SPEED * dt;
		target = position + view;	
	}

		
	position += view * ZOOM_SPEED * dt;
		
	target = position + view;
	//if(Application::IsKeyPressed('S'))
	//{
	//	position -= view * ZOOM_SPEED * dt;
	//	target = position + view;
	//}

	{
		if (position.z >= 100)
		{
			position.z = 100;
			target = position + view;
		}
		else if (position.z <= -100)
		{
			position.z = -100;
			target = position + view;
		}
	}

	if (Application::IsKeyPressed('R') && retry)
	{
		Reset();
	}


	if (position.y > 10)
	{
		position.y = 10 + jumpHeight;
	}
	target = position + view;
}


void Camera_Obstacle::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}
