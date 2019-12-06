#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	std::vector<glm::vec3> getVertices() const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	const glm::mat4x4 getWorldTransformation();

	void setScale(float newScale);

	void setRotate(glm::vec3 rotateVector);
	

private:
	//func
	const glm::mat4x4 getRotationMatrix();
	const glm::mat4x4 getTranslationMatrix();
	const glm::mat4x4 getScalingMatrix();

	// default memebers
	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::string model_name_;
	
	// matrices
	glm::mat4x4 worldTransform;
	glm::mat4x4 rotateTransform;

	// vecs
	glm::vec3 translateVector;
	glm::vec3 scaleVector;
	glm::vec3 rotateVector;

};
