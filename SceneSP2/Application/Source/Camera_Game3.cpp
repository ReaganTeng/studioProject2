#include "Camera_Game3.h"
#include "Application.h"
#include "Mtx44.h"
#include <math.h>

Camera_Game3::Camera_Game3()
{
}

Camera_Game3::~Camera_Game3()
{
}

void Camera_Game3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera_Game3::UpdateCamCollision(std::vector<float> mazeWallPos,std::vector<float> mazeWallRotate, std::vector<float> mazeWallSize,
	std::vector<float> digitPos, std::vector<float> digitSize, Vector3& prevPos)
{
	float halfdigitLength = 12;
	float halfdigitWidth = 7;

	float mazeWall3Length = 370;
	float mazeWall3Width = 34;
	//Wall Collision (unsigned start at 12 cos of boundary)
	//Size3 wall 1 [12-15]
	for (unsigned i = 12; i < 15; i += 3)
	{
		if (position.x > mazeWallPos[i] && position.x < mazeWallPos[i] + mazeWall3Width &&
			position.z > mazeWallPos[i + 2] - mazeWall3Length && position.z < mazeWallPos[i + 2])
		{
			position.x = prevPos.x;
			position.z = prevPos.z;
		}
	}
	//Size3 wall 2 [15-18]
	for (unsigned i = 15; i < 18; i += 3)
	{
		if (position.x < mazeWallPos[i] && position.x > mazeWallPos[i] - mazeWall3Length &&
			position.z > mazeWallPos[i + 2] - mazeWall3Width && position.z < mazeWallPos[i + 2])
		{
			position.x = prevPos.x;
			position.z = prevPos.z;
		}
	}
	//Size3 wall 3-4 [18-24]
	for (unsigned i = 18; i < 24; i += 3)
	{
		if (position.x > mazeWallPos[i] && position.x < mazeWallPos[i] + mazeWall3Width &&
			position.z > mazeWallPos[i + 2] - mazeWall3Length && position.z < mazeWallPos[i + 2])
		{
			position.x = prevPos.x;
			position.z = prevPos.z;
		}
	}
	//Size 3 wall 5[24-27]
	for (unsigned i = 24; i < 27; i += 3)
	{
		if (position.x < mazeWallPos[i] && position.x > mazeWallPos[i] - mazeWall3Length &&
			position.z > mazeWallPos[i + 2] - mazeWall3Width && position.z < mazeWallPos[i + 2])
		{
			position.x = prevPos.x;
			position.z = prevPos.z;
		}
	}

	//Size 2 wall 6 [27-30]
	for (unsigned i = 27; i < 30; i += 3)
	{
		if (position.x > mazeWallPos[i] && position.x < mazeWallPos[i] + (mazeWall3Length / 2) &&
			position.z < mazeWallPos[i + 2] + mazeWall3Width && position.z > mazeWallPos[i + 2])
		{
			position.x = prevPos.x;
			position.z = prevPos.z;
		}
	}
	//Size 2 wall 7 [30-33]
	for (unsigned i = 30; i < 33; i += 3)
	{
		if (position.x > mazeWallPos[i] && position.x < mazeWallPos[i] + mazeWall3Width &&
			position.z > mazeWallPos[i + 2] - (mazeWall3Length / 2) && position.z < mazeWallPos[i + 2])
		{
			position.x = prevPos.x;
			position.z = prevPos.z;
		}
	}
	//Size 2 wall 8,9 [33-39]
	for (unsigned i = 33; i < 39; i += 3)
	{
		if (position.x < mazeWallPos[i] && position.x > mazeWallPos[i] - mazeWall3Width &&
			position.z < mazeWallPos[i + 2] + (mazeWall3Length / 2) && position.z > mazeWallPos[i + 2])
		{
			position.x = prevPos.x;
			position.z = prevPos.z;
		}
	}
	//Size 2 wall 10 [39-42]
	for (unsigned i = 39; i < 42; i += 3)
	{
		if (position.x > mazeWallPos[i] && position.x < mazeWallPos[i] + mazeWall3Width &&
			position.z > mazeWallPos[i + 2] - (mazeWall3Length / 2) && position.z < mazeWallPos[i + 2])
		{
			position.x = prevPos.x;
			position.z = prevPos.z;
		}
	}
	//Size 2 wall 11 [42-45]
	for (unsigned i = 42; i < 45; i += 3)
	{
		if (position.x > mazeWallPos[i] && position.x < mazeWallPos[i] + (mazeWall3Length / 2) &&
			position.z < mazeWallPos[i + 2] + mazeWall3Width && position.z > mazeWallPos[i + 2])
		{
			position.x = prevPos.x;
			position.z = prevPos.z;
		}
	}
	//Size 2 wall 12 [45-48]
	for (unsigned i = 45; i < 48; i += 3)
	{
		if (position.x > mazeWallPos[i] && position.x < mazeWallPos[i] + mazeWall3Width &&
			position.z > mazeWallPos[i + 2] - (mazeWall3Length / 2) && position.z < mazeWallPos[i + 2])
		{
			position.x = prevPos.x;
			position.z = prevPos.z;
		}
	}
	//Size 2 wall 13 [48-51]
	for (unsigned i = 48; i < 51; i += 3)
	{
		if (position.x < mazeWallPos[i] && position.x > mazeWallPos[i] - (mazeWall3Length / 2) &&
			position.z > mazeWallPos[i + 2] - mazeWall3Width && position.z < mazeWallPos[i + 2])
		{
			position.x = prevPos.x;
			position.z = prevPos.z;
		}
	}


	//Digit collision
	for (unsigned i = 0; i < digitPos.size(); i += 3)
	{
		if (digitPos[i] + halfdigitLength > position.x && position.x > digitPos[i] - halfdigitLength &&
			digitPos[i + 2] + halfdigitWidth > position.z && position.z > digitPos[i + 2] - halfdigitWidth ||
			digitPos[i] - halfdigitLength < position.x && position.x < digitPos[i] + halfdigitLength &&
			digitPos[i + 2] - halfdigitWidth < position.z && position.z < digitPos[i + 2] + halfdigitWidth)
		{
			position.x = prevPos.x;
			position.z = prevPos.z;
		}
	}
}

void Camera_Game3::Update(double dt, std::vector<float> NPCPos, std::vector<float> NPCSize, std::vector<float> mazeWallRotate,
	std::vector<float> tablePos, std::vector<float> tableSize)
{
	static const float CAMERA_SPEED = 80;
	static const float ZOOM_SPEED = 85;

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);

	if(Application::IsKeyPressed('A'))
	{
		Vector3 prevPos = position;
		position -= right * ZOOM_SPEED * dt;
		position.y = 25;
		UpdateCamCollision(NPCPos, NPCSize, mazeWallRotate, tablePos, tableSize, prevPos);
		target = position + view;
	}
	if(Application::IsKeyPressed('D'))
	{
		Vector3 prevPos = position;
		position += right * ZOOM_SPEED * dt;
		position.y = 25;
		UpdateCamCollision(NPCPos, NPCSize, mazeWallRotate, tablePos, tableSize, prevPos);
		target = position + view;	
	}
	if(Application::IsKeyPressed('W'))
	{
		Vector3 prevPos = position;
		position += view * ZOOM_SPEED * dt;
		position.y = 25;
		UpdateCamCollision(NPCPos, NPCSize, mazeWallRotate, tablePos, tableSize, prevPos);
		target = position + view;
	}
	if(Application::IsKeyPressed('S'))
	{
		Vector3 prevPos = position;
		position -= view * ZOOM_SPEED * dt;
		position.y = 25;
		UpdateCamCollision(NPCPos, NPCSize, mazeWallRotate, tablePos, tableSize, prevPos);
		target = position + view;
	}

	{ //Scene Collision
		if (position.x >= 426)
		{
			position.x = 426;
			target = position + view;
		}
		else if (position.x <= -426)
		{
			position.x = -426;
			target = position + view;
		}
		if (position.z <= -432)
		{
			position.z = -432;
			target = position + view;
		}
		else if (position.z >= 432)
		{
			position.z = 432;
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

