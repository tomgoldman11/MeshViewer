#pragma once
#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"
#include "Camera.h"
#include "LightSource.h"


class Utils
{
public:
	
	static glm::vec4 Vec4FromVec3(const glm::vec3 & vector3);
	static glm::vec3 Vec3fFromStream(std::istream& issLine);
	static glm::vec2 Vec2fFromStream(std::istream& issLine);
	static std::shared_ptr<MeshModel> LoadMeshModel(const std::string& filePath);
	static std::shared_ptr<Camera> LoadCamera(const glm::vec3 _eye, const glm::vec3 _at, const glm::vec3 _up);
	static std::shared_ptr<LightSource> LoadLight(glm::vec3 _position, glm::vec3 _color, bool _pointSource, glm::vec3 _target);
	static std::string GetFileName(const std::string& filePath);
	static bool  Utils::PointInTriangle(glm::vec2 pt, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, float * zPoint);
	static bool Utils::getLinearInterpolationOfPoints(float x, float y, const glm::vec3 & point1, const glm::vec3 & point2, const glm::vec3 & point3, float * const alpha, float * const beta, float * const gama, bool * const changed);
	static std::vector<glm::vec3> CalculateNormals(std::vector<glm::vec3> vertices, std::vector<Face> faces);
	static float Utils::sign(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
};
