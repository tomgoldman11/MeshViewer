#include "Camera.h"

Camera::Camera(const glm::vec3 & eye, const glm::vec3 & at, const glm::vec3 & up) :
	curPos({eye,at,up}),
	orthoView({ -1.0f ,1.0f ,-1.0f ,1.0f ,-1.0f ,1.0f }), // left, right, bottom, top, near, far
	perspView({ 1.0f, 100.0f , 0.01f, 10.0f }), // aspect, fovy, near, far
	zoom(2.0f)
{
	setCameraLookAt(eye, at, up);
	projection_transformation_= glm::mat4x4(1);

	//projection_transformation_ = glm::perspective(perspView.fovy * 0.01745329251994329576923690768489f, perspView.aspect, perspView._near, perspView._far);
	//setPerspectiveProjection();
	setOrthographicProjection();
	//setPerspectiveProjection_Alter();
}

Camera::~Camera()
{

}

void Camera::setCameraLookAt(const glm::vec3 & eye, const glm::vec3 & at, const glm::vec3 & up)
{
	curPos.eye = eye; curPos.at = at;
	glm::vec3 zaxis = glm::normalize(eye - at);
	glm::vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
	glm::vec3 yaxis = glm::normalize(glm::cross(zaxis, xaxis));

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
		{-eye.x,-eye.y,-eye.z,1} );

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
	setPerspectiveProjection(perspView.aspect,  perspView.fovy,  perspView._near, perspView._far);
}

void Camera::setPerspectiveProjection(const float aspect, const float fovy, const float near,const float far)
{
	perspView.aspect = aspect;
	perspView.fovy = fovy;
	perspView._near = near;
	perspView._far = far;
	const float zRange = far -near;

	const float fov = fovy * 0.01745329251994329576923690768489f / zoom;

	float tanHalfFOV = tan(fov / 2.0f);

	glm::mat4x4 result(1);

	//result[0][0] = 1.0f / (tanHalfFOV * aspect);
	//result[1][1] = 1.0f / (tanHalfFOV);
	//result[2][2] = -(far + near) / zRange;
	//result[3][2] = -(2.0f * far * near) / zRange;
	//result[2][3] = -1.0f;
	//projection_transformation_ = result;

	const float height = near*(tan(fov / 2.0f));
	const float width = aspect * height;

	result[0][0] = near/width;
	result[1][1] = near / height;
	result[2][2] = -(far + near) / zRange;
	result[3][2] = -(2.0f * far * near) / zRange ;
	result[2][3] = -1.0f;

	projection_transformation_ = glm::transpose(result);

	//float scale = tan(fovy * 0.5 * 0.01745329251994329576923690768489f) * near;
	//float _projRight = aspect * scale;
	//float _projLeft = -_projRight;
	//float _projTop = scale, _projBottom = -_projTop;

	//glm::mat4x4 M;
	//M[0][0] = 2 * near / (_projRight - _projLeft);
	//M[0][1] = 0;
	//M[0][2] = 0;
	//M[0][3] = 0;

	//M[1][0] = 0;
	//M[1][1] = 2 * near / (_projTop - _projBottom);
	//M[1][2] = 0;
	//M[1][3] = 0;

	//M[2][0] = (_projRight + _projLeft) / (_projRight - _projLeft);
	//M[2][1] = (_projTop + _projBottom) / (_projTop - _projBottom);
	//M[2][2] = -(far + near) / (far - near);
	//M[2][3] = -1;

	//M[3][0] = 0;
	//M[3][1] = 0;
	//M[3][2] = -2 * far * near / (far - near);
	//M[3][3] = 0;

	//projection_transformation_ = M;
}

void Camera::setPerspectiveProjection_Alter()
{
	setPerspectiveProjection_Alter(perspView.aspect, perspView.fovy, perspView._near, perspView._far);
}
void Camera::setPerspectiveProjection_Alter(const float aspectRatio, const float fovy, const float near, const float far)
{
	float scale = tan(fovy * 0.5 * M_PI / 180);
	frustumView.top = near * scale, frustumView.bottom = -frustumView.top;
	frustumView.right = frustumView.top * aspectRatio;
	frustumView.left = -frustumView.right;
	frustumView._near = near, frustumView._far = far;

	glm::mat4 normalization = glm::mat4(
		{ (-2 / (frustumView.right - frustumView.left)),0,0,0 },
		{ 0,(2 / (frustumView.top - frustumView.bottom)),0,0 },
		{ 0,0,(2 / (frustumView._near - frustumView._far)),0 },
		{ -((frustumView.right + frustumView.left) / (frustumView.right - frustumView.left)),	-((frustumView.top + frustumView.bottom) / (frustumView.top - frustumView.bottom)),	-((frustumView._far + frustumView._near) / (frustumView._far - frustumView._near)),	1 }
	);

	glm::mat4x4 M;
	M[0][0] = (2 * frustumView._near) / (frustumView.right - frustumView.left);
	M[0][1] = 0;
	M[0][2] = 0;
	M[0][3] = 0;

	M[1][0] = 0;
	M[1][1] = (2 * frustumView._near) / (frustumView.top - frustumView.bottom);
	M[1][2] = 0;
	M[1][3] = 0;

	M[2][0] = (frustumView.right + frustumView.left) / (frustumView.right - frustumView.left);
	M[2][1] = (frustumView.top + frustumView.bottom) / (frustumView.top - frustumView.bottom);
	M[2][2] = -(frustumView._far + frustumView._near) / (frustumView._far - frustumView._near);
	M[2][3] = -1;

	M[3][0] = 0;
	M[3][1] = 0;
	M[3][2] = -(2 * frustumView._far * frustumView._near) / (frustumView._far - frustumView._near);
	M[3][3] = 0;

	projection_transformation_ = M * normalization;
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

void Camera::setFOVY(const float _fovy)
{
	perspView.fovy = _fovy;
	//setPerspectiveProjection();
}

float Camera::getFOVY() const
{
	return perspView.fovy;
}

void Camera::setAspectRatio(const float _aspectRatio)
{
	perspView.aspect = _aspectRatio;
	//setPerspectiveProjection();
}

void Camera::setNear(const float _near)
{
	perspView._near = _near;
	//setPerspectiveProjection();
}

void Camera::setFar(const float _far)
{
	perspView._far = _far;
	//setPerspectiveProjection();
}

void Camera::setZoom(const float _zoom)
{
	zoom = _zoom;
	orthoView.left = orthoView.left / zoom;
	orthoView.right = orthoView.right / zoom;
	orthoView.bottom = orthoView.bottom / zoom;
	orthoView.top = orthoView.top / zoom;
}

float Camera::getZoom() const
{
	return zoom;
}

void Camera::set_current_position(const glm::vec3 _eye, const glm::vec3 _at, const glm::vec3 _up)
{
	curPos.eye = _eye;
	curPos.at = _at;
	curPos.up = _up;
}

glm::vec3 Camera::getEye() const
{
	return curPos.eye;
}

glm::vec3 Camera::getAt() const
{
	return curPos.at;
}

glm::vec3 Camera::getUp() const
{
	return curPos.up;
}
