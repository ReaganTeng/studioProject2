#ifndef CAMERA_GAME5_H
#define CAMERA_GAME5_H

#include "Camera.h"
#include <vector>

class Camera_Game5: public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	int boundary;
	float sprintspeed;
	int speedDir;
	float speedtimer;
	Camera_Game5();
	~Camera_Game5();
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Update(std::vector<float> tablePos, double dt);
	void BuildingCollision(std::vector<float> tablePos, Vector3 prevPos);
};

#endif