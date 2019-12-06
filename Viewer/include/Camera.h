#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

struct ortho {
	float left;
	float right;
	float bottom;
	float top;
	float near;
	float far;
};

struct frustum {
	float left;
	float right;
	float bottom;
	float top;
	float near;
	float far;
};

struct perspective {
	float fovy;
	float aspect;
	float near;
	float far;
};

class Camera
{
public:
	Camera(const glm::vec3 & eye, const glm::vec3 & at, const glm::vec3 & up);
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4 GetViewTransformation() const;

private:
	glm::mat4x4 view_transformation_;
	glm::mat4x4 projection_transformation_;

	
};
