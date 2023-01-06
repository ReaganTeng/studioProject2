#ifndef CAMERA_OBSTACLE_H
#define CAMERA_OBSTACLE_H

#include "Camera.h"

class Camera_Obstacle: public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Camera_Obstacle();
	~Camera_Obstacle();
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Update(double dt, float jumpHeight, bool retry);
	void Reset();
};

#endif