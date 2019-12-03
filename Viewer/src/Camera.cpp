#include "Camera.h"

Camera::Camera(const glm::vec3 & eye, const glm::vec3 & at, const glm::vec3 & up)
{
	SetCameraLookAt(eye, at, up);
	projection_transformation_ = glm::mat4x4(1);
}

Camera::~Camera()
{

}

void Camera::SetCameraLookAt(const glm::vec3 & eye, const glm::vec3 & at, const glm::vec3 & up)
{
	glm::vec3 z = glm::normalize(eye - at);
	glm::vec3 x = glm::normalize(glm::cross(up, z));
	glm::vec3 y = glm::normalize(glm::cross(z, x));
	glm::mat4x4 R = { {x,0}, {y,0}, {z,0},{0.0,0.0,0.0,1.0} };
	glm::mat4x4 eyeTranslate = {
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{-eye.x,-eye.y,-eye.z,1} };

	view_transformation_ = eyeTranslate * R;
	//glm::mat4x4 viewMatrix(1);
	//
	//viewMatrix[0][0] = x.x;
	//viewMatrix[1][0] = x.y;
	//viewMatrix[2][0] = x.z;
	//viewMatrix[0][1] = y.x;
	//viewMatrix[1][1] = y.y;
	//viewMatrix[2][1] = y.z;
	//viewMatrix[0][2] = -z.x;
	//viewMatrix[1][2] = -z.y;
	//viewMatrix[2][2] = -z.z;
	//viewMatrix[3][0] = 0.0f;
	//viewMatrix[3][1] = 0.0f;
	//viewMatrix[3][2] = 0.0f;
	//viewMatrix[3][3] = 1.0f;
	//viewMatrix[0][3] = -glm::dot(x, eye);
	//viewMatrix[1][3] = -glm::dot(y, eye);
	//viewMatrix[2][3] = -glm::dot(z, eye);

	//view_transformation_ = viewMatrix;

}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation_;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation_;
}