#include "Camera.h"

Camera::Camera(const glm::vec3 & eye, const glm::vec3 & at, const glm::vec3 & up) :
	orthoView({ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f })
{
	setCameraLookAt(eye, at, up);
	setPerspectiveProjection(60.0f, 4.0f / 3.0f, 0.1f, 9.0f);
}

Camera::~Camera()
{

}

void Camera::setCameraLookAt(const glm::vec3 & eye, const glm::vec3 & at, const glm::vec3 & up)
{
	glm::vec3 zaxis = glm::normalize(eye - at);
	glm::vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
	glm::vec3 yaxis = glm::cross(zaxis, xaxis);
	glm::mat4x4 c = glm::mat4x4(
		{ xaxis.x,xaxis.y,xaxis.z,0.0},
		{ yaxis.x,yaxis.y,yaxis.z,0.0},
		{ zaxis.x, zaxis.y, zaxis.z ,0.0},
		{0.0,0.0,0.0,1.0} 
	);
	glm::mat4x4 eyeTranslate = glm::mat4x4(
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{-eye.x,-eye.y,-eye.z,1} ); // - or not

	view_transformation_ = c * eyeTranslate;
}

const glm::mat4x4 Camera::getProjectionTransformation() const
{
	return projection_transformation_;
}

const glm::mat4x4 Camera::getViewTransformation() const
{
	return view_transformation_;
}


void Camera::setPerspectiveProjection(const float fovy, const float aspect,const float near,const float far)
{
	perspView.fovy = fovy;
	perspView.aspect = aspect;
	perspView._near = near;
	perspView._far = far;

	float fov = fovy * 0.01745329251994329576923690768489f;

	float tanHalfFovy = tan(fov / 2.0f);

	glm::mat4x4 result(1);

	result[0][0] = 1.0f / (aspect * tanHalfFovy);
	result[1][1] = 1.0f / (tanHalfFovy);
	result[2][2] = (far + near) / (near - far);
	result[2][3] = -1.0f;
	result[3][2] = (2.0f * far * near) / (near - far);

	projection_transformation_ = result;

}

