#include "Camera_Game1.h"
#include "Application.h"
#include "Mtx44.h"
#include <math.h>

Camera_Game1::Camera_Game1()
{
}

Camera_Game1::~Camera_Game1()
{
}

void Camera_Game1::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera_Game1::UpdateCamCollision(std::vector<float> NPCPos, std::vector<float> NPCSize,
	std::vector<float> tablePos, std::vector<float> tableSize, Vector3& prevPos)
{
	float halfTableLength = 12;
	float halfTableWidth = 7;
	float halfNPCLength = 8;
	float halfScamLength = 4;
	//Collision
	for (unsigned i = 0; i < NPCPos.size(); i += 3)
	{
		if (NPCPos[i] + halfNPCLength > position.x && position.x > NPCPos[i] - halfNPCLength &&
			NPCPos[i + 2] + halfNPCLength > position.z && position.z > NPCPos[i + 2] - halfNPCLength ||
			NPCPos[i] - halfNPCLength < position.x && position.x < NPCPos[i] + halfNPCLength &&
			NPCPos[i + 2] - halfNPCLength < position.z && position.z < NPCPos[i + 2] + halfNPCLength)
		{
			position.x = prevPos.x;
			position.z = prevPos.z;
		}
	}
	for (unsigned i = 0; i < tablePos.size(); i += 3)
	{
		if (tablePos[i] + halfTableLength > position.x && position.x > tablePos[i] - halfTableLength &&
			tablePos[i + 2] + halfTableWidth > position.z && position.z > tablePos[i + 2] - halfTableWidth ||
			tablePos[i] - halfTableLength < position.x && position.x < tablePos[i] + halfTableLength &&
			tablePos[i + 2] - halfTableWidth < position.z && position.z < tablePos[i + 2] + halfTableWidth)
		{
			position.x = prevPos.x;
			position.z = prevPos.z;
		}
	}
}

void Camera_Game1::Update(double dt, std::vector<float> NPCPos, std::vector<float> NPCSize,
	std::vector<float> tablePos, std::vector<float> tableSize)
{
	static const float CAMERA_SPEED = 80;
	static const float ZOOM_SPEED = 40;

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);

	if(Application::IsKeyPressed('A'))
	{
		Vector3 prevPos = position;
		position -= right * ZOOM_SPEED * dt;
		position.y = 25;
		UpdateCamCollision(NPCPos, NPCSize, tablePos, tableSize, prevPos);
		target = position + view;
	}
	if(Application::IsKeyPressed('D'))
	{
		Vector3 prevPos = position;
		position += right * ZOOM_SPEED * dt;
		position.y = 25;
		UpdateCamCollision(NPCPos, NPCSize, tablePos, tableSize, prevPos);
		target = position + view;	
	}
	if(Application::IsKeyPressed('W'))
	{
		Vector3 prevPos = position;
		position += view * ZOOM_SPEED * dt;
		position.y = 25;
		UpdateCamCollision(NPCPos, NPCSize, tablePos, tableSize, prevPos);
		target = position + view;
	}
	if(Application::IsKeyPressed('S'))
	{
		Vector3 prevPos = position;
		position -= view * ZOOM_SPEED * dt;
		position.y = 25;
		UpdateCamCollision(NPCPos, NPCSize, tablePos, tableSize, prevPos);
		target = position + view;
	}

	{ //Scene Collision
		if (position.x >= 85)
		{
			position.x = 85;
			target = position + view;
		}
		else if (position.x <= -85)
		{
			position.x = -85;
			target = position + view;
		}
		if (position.z <= -85)
		{
			position.z = -85;
			target = position + view;
		}
		else if (position.z >= 85)
		{
			position.z = 85;
			target = position + view;
		}
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
	
}

