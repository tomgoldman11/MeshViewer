#include "Camera.h"

Camera::Camera(const glm::vec3 & eye, const glm::vec3 & at, const glm::vec3 & up) :
	orthographicPrams(0.0f)

{
	SetCameraLookAt(eye, at, up);
	orthographicMatrix = 
}

Camera::~Camera()
{

}

void Camera::SetCameraLookAt(const glm::vec3 & eye, const glm::vec3 & at, const glm::vec3 & up)
{
	glm::vec3 z = glm::normalize(at - eye);
	glm::vec3 x = glm::normalize(glm::cross(up, z));
	glm::vec3 y = glm::cross(z, x);
	glm::mat4x4 c = { {x,0.0}, {y,0.0}, {-z,0.0},{0.0,0.0,0.0,1.0} };
	glm::mat4x4 eyeTranslate = glm::mat4x4({
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{-eye.x,-eye.y,-eye.z,1} }); // - or not

	view_transformation_ = c * eyeTranslate;

}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation_;
}

const glm::mat4x4 Camera::GetViewTransformation() const
{
	return view_transformation_;
}