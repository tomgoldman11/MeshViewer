#pragma once
#include <glm/glm.hpp>
#include <string>
#include <algorithm>
#include "Face.h"
#include <map>

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
	std::vector<glm::vec3> getNormals() const;
	void setScale(float newScale);
	const int getVertexFacesSum(int indx) const;
	glm::vec3 getRotateVector() const;

	// setters
	void setScale(glm::vec3 newScale);
	void setRotate(glm::vec3 newRotate);
	void setTranslate(glm::vec3 newTranslate);

	

	void setFaceNormals();
	

private:
	// getters
	glm::mat4x4 getRotationMatrix();
	glm::mat4x4 getTranslationMatrix();
	glm::mat4x4 getScalingMatrix();


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

	// model points attributes
	glm::vec3 buttom;
	glm::vec3 top;
	glm::vec3 centerPoint;
	std::map<int, int> verticesFaces;
};
