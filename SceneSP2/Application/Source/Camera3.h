#ifndef CAMERA3_H
#define CAMERA3_H

#include "Camera.h"

class Camera3: public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Camera3();
	~Camera3();
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Update(double dt);
};

#endif