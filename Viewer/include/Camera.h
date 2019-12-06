#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"


class Camera
{
public:
	struct ortho {
		float left;
		float right;
		float bottom;
		float top;
		float _near;
		float _far;
	};

	struct frustum {
		float left;
		float right;
		float bottom;
		float top;
		float _near;
		float _far;
	};

	struct perspective {
		float fovy;
		float aspect;
		float _near;
		float _far;
	};

	Camera(const glm::vec3 & eye, const glm::vec3 & at, const glm::vec3 & up);
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4 GetViewTransformation() const;

	void SetPerspectiveProjection(const float fovy, const float aspectRatio, const float, const float);

private:
	glm::mat4x4 view_transformation_;
	glm::mat4x4 projection_transformation_;
	ortho orthoView;
	perspective perspView;
};
