#include "Camera.h"
#include "Application.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera::Camera()
{
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera::~Camera()
{
}

/******************************************************************************/
/*!
\brief
Initialize camera

\param pos - position of camera
\param target - where the camera is looking at
\param up - up vector of camera
*/
/******************************************************************************/
void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
}

/******************************************************************************/
/*!
\brief
Reset the camera settings
*/
/******************************************************************************/
void Camera::Reset()
{
}

/******************************************************************************/
/*!
\brief
To be called every frame. Camera will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/
void Camera::Update(double dt)
{
	/*x(phi, theta) = R.cos(phi).cos(theta)
	y(phi, thetha) = R.sin(phi)
	z(phi, theta) = R.cos(phi).sin(theta)*/

	float degreePerStack = 180.f / 30;
	float degreePerSlice = 360.f / 30;

	static const float CAMERA_SPEED = 20.f;

	for (unsigned stack = 0; stack <= 30; ++stack)
	{
		float phi = -90.f + degreePerStack * stack;

		

		for (unsigned slice = 0; slice <= 30; ++slice)
		{
			float theta = degreePerSlice * slice;

			if (Application::IsKeyPressed('W'))
				this->position.y += CAMERA_SPEED * sin(Math::DegreeToRadian(phi)) * static_cast<float>(dt) ; //move camera up
			if (Application::IsKeyPressed('A'))
				this->position.x -= CAMERA_SPEED * sin(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta)) *static_cast<float>(dt) ; //move camera left
			if (Application::IsKeyPressed('S'))
				this->position.y -= CAMERA_SPEED * sin(Math::DegreeToRadian(phi)) * static_cast<float>(dt) ; //move camera down
			if (Application::IsKeyPressed('D'))
				this->position.x += CAMERA_SPEED * sin(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta))  * static_cast<float>(dt) ;  //move camera right
		}
	}

	
}