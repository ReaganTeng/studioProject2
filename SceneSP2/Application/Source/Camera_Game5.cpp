#include "Camera_Game5.h"
#include "Application.h"
#include "Mtx44.h"
#include <vector>

Camera_Game5::Camera_Game5()
{
}

Camera_Game5::~Camera_Game5()
{
}

void Camera_Game5::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	sprintspeed = 0;	
	boundary = 700;
	speedtimer = 5;
}
void Camera_Game5::BuildingCollision(std::vector<float> tablePos, Vector3 prevPos)
{
	float halftable = 10;
	float halfbuildingz = 517;
	float halfbuildingx = 250;
	for (unsigned i = 0;i < tablePos.size();i += 3)
	{
		if (tablePos[i] + halftable > position.x && tablePos[i] - halftable < position.x &&
			tablePos[i + 2] + halftable > position.z && tablePos[i + 2] - halftable < position.z)
		{
			position.x = prevPos.x;
			position.z = prevPos.z;
		}
	}
	//building collision for buildings 1-8
	if (-325 + halfbuildingx > position.x && -325 - halfbuildingx < position.x &&
		95 + halfbuildingz>position.z && 95 - halfbuildingz < position.z)
	{
		position.x = prevPos.x;
		position.z = prevPos.z;
	}
	//building collision for building 9-16
	if (375 + halfbuildingx > position.x && 375 - halfbuildingx < position.x &&
		95 + halfbuildingz>position.z && 95 - halfbuildingz < position.z)
	{
		position.x = prevPos.x;
		position.z = prevPos.z;
	}
}
void Camera_Game5::Update(std::vector<float> tablePos, double dt)
{

	static const float CAMERA_SPEED = 80;
	static const float ZOOM_SPEED = 80;

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);

	if(Application::IsKeyPressed('A'))
	{
		Vector3 prevPos = position;
		position.y = 20;
		position -= right * ZOOM_SPEED * dt;
		BuildingCollision(tablePos, prevPos);
		target = position + view;
	}
	if(Application::IsKeyPressed('D'))
	{
		Vector3 prevPos = position;
		position.y = 20;
		position += right * ZOOM_SPEED * dt;
		BuildingCollision(tablePos, prevPos);
		target = position + view;	
	}
	if(Application::IsKeyPressed('W'))
	{
		Vector3 prevPos = position;
		position.y = 20;
		position += view * ZOOM_SPEED * dt;
		BuildingCollision(tablePos, prevPos);
		target = position + view;
	}
	if(Application::IsKeyPressed('S'))
	{
		Vector3 prevPos = position;
		position.y = 20;
		position -= view * ZOOM_SPEED * dt;
		BuildingCollision(tablePos, prevPos);
		target = position + view;
	}

	if (position.x > boundary)
	{
		position.x = boundary;
		target = position + view;
	}
	else if (position.x < -boundary)
	{
		position.x = -boundary;
		target = position + view;
	}
	if (position.z > boundary)
	{
		position.z = boundary;
		target = position + view;
	}
	else if (position.z < -boundary)
	{
		position.z = -boundary;
		target = position + view;
	}

	

	if (Application::IsKeyPressed(VK_UP))
	{
		Mtx44 rotation;
		float pitch = CAMERA_SPEED * static_cast<float>(dt);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		Mtx44 rotation;
		float pitch = -CAMERA_SPEED * static_cast<float>(dt);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	if (Application::IsKeyPressed(VK_LEFT))
	{
		Mtx44 rotation;
		float yaw = CAMERA_SPEED * static_cast<float>(dt);
		rotation.SetToRotation(yaw, 0, 1, 0);
		up = rotation * up;
		view = rotation * view;
		target = position + view;

	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		Mtx44 rotation;
		float yaw = -CAMERA_SPEED * static_cast<float>(dt);
		rotation.SetToRotation(yaw, 0, 1, 0);
		up = rotation * up;
		view = rotation * view;
		target = position + view;

	}
	//if (Application::IsKeyPressed(VK_SHIFT))
	//{
	//	sprintspeed += (float)(20 * dt);
	//	if (sprintspeed >= 100)
	//	{
	//		sprintspeed = 100;
	//		speedtimer
	//	}

	//}


}



