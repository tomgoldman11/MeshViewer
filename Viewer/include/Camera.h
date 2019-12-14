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
		float aspect;
		float fovy;
		float _near;
		float _far;
	};

	struct position {
		glm::vec3 eye;
		glm::vec3 at;
		glm::vec3 up;
	};
	Camera(const glm::vec3 & eye, const glm::vec3 & at, const glm::vec3 & up);
	virtual ~Camera();

	void setCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	const glm::mat4x4 getProjectionTransformation() const;
	const glm::mat4x4 getViewTransformation() const;

	void setPerspectiveProjection();

	void setPerspectiveProjection(const float fovy, const float aspectRatio, const float near, const float far);

	void setOrthographicProjection();

	void setOrthographicProjection(const float left, const float right, const float bottom, const float top, const float _near, const float _far);

	void setFOVY(const float _fovy);
	void setAspectRatio(const float _aspectRatio);
	void setNear(const float _near);
	void setFar(const float _far);
	void setZoom(const float _zoom);
	void set_current_position(const glm::vec3 eye, const glm::vec3 at, const glm::vec3 up);
	glm::vec3 getEye()const;
	glm::vec3 getAt()const;
	glm::vec3 getUp()const;

private:
	glm::mat4x4 view_transformation_;
	glm::mat4x4 projection_transformation_;
	ortho orthoView;
	perspective perspView;
	position curPos;
	float zoom;
};
