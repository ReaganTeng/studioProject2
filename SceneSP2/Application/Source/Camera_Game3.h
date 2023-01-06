#ifndef CAMERA_Game3_H
#define CAMERA_Game3_H

#include "Camera.h"
#include <vector>

class Camera_Game3: public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Camera_Game3();
	~Camera_Game3();
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Update(double dt, std::vector<float> NPCPos, std::vector<float> NPCSize, std::vector<float> mazeWallRotate,
		std::vector<float> tablePos, std::vector<float> tableSize);
	void UpdateCamCollision(std::vector<float> NPCPos, std::vector<float> NPCSize, std::vector<float> mazeWallSize,
							std::vector<float> tablePos, std::vector<float> tableSize, Vector3& prevPos);
};

#endif