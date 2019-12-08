#include "Camera.h"

Camera::Camera(const glm::vec3 & eye, const glm::vec3 & at, const glm::vec3 & up) :
	orthoView({ -1.0f ,1.0f ,-1.0f ,1.0f ,-1.0f ,1.0f }),
	perspView({ 60.0f, 4.0f / 3.0f, 0.1f, 9.0f })
{
	setCameraLookAt(eye, at, up);
	projection_transformation_= glm::mat4x4(1);
	setOrthographicProjection();
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

void Camera::setPerspectiveProjection()
{
	setPerspectiveProjection(perspView.fovy, perspView.aspect, perspView._near, perspView._far);
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
	result[2][2] = -(far + near) / (near - far);
	result[2][3] = -1.0f;
	result[3][2] = (2.0f * far * near) / (near - far);

	projection_transformation_ = result;
}

void Camera::setOrthographicProjection()
{
	setOrthographicProjection(orthoView.left, orthoView.right, orthoView.bottom, orthoView.top, orthoView._near, orthoView._far);
}

void Camera::setOrthographicProjection(const float left, const float right, const float bottom, const float top, const float _near, const float _far)
{
	projection_transformation_ = glm::mat4x4(
		{2.0f / (right - left) ,0,0,0},
		{0,2.0f / (top - bottom) ,0,0},
		{0,0,2.0f / (_near - _far),0},
		{ -(right + left) / (right - left) , -(bottom + top) / (top - bottom) , -(_far + _near) / (_far - _near),1}
	);
}

