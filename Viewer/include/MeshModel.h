#pragma once
#include <glm/glm.hpp>
#include <string>
#include <algorithm>
#include "Face.h"
#include <map>

class MeshModel
{
public:
	struct box {
		glm::vec3 XnYZ;
		glm::vec3 XnYnZ;
		glm::vec3 nXnYnZ; 
		glm::vec3 nXnYZ; 
		glm::vec3 XYZ; 
		glm::vec3 XYnZ; 
		glm::vec3 nXYnZ;
		glm::vec3 nXYZ;
	};
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	std::vector<glm::vec3> getVertices() const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	const glm::mat4x4 getWorldTransformation();
	const glm::mat4x4 getLocalTransformation();
	std::vector<glm::vec3> getNormals() const;
	const int getVertexFacesSum(int indx) const;
	void getModelBoxVetrtices(glm::vec3& XnYZ, glm::vec3& XnYnZ, glm::vec3& nXnYnZ, glm::vec3& nXnYZ, glm::vec3& XYZ, glm::vec3& XYnZ, glm::vec3& nXYnZ, glm::vec3& nXYZ) const;

	

	// setters
	void setScale(float newScale);
	void setScale(const glm::vec3 newScale);
	void setRotate(const glm::vec3 newRotate);
	void setTranslate(const glm::vec3 newTranslate);
	void setScale_local(const float newScale);
	void setScale_local(const glm::vec3 newScale);
	void setRotate_local(const glm::vec3 newRotate);
	void setTranslate_local(const glm::vec3 newTranslate);
	void setFaceNormals();

	// getters
	glm::vec3 getRotateVector_world() const;
	glm::vec3 getScaleVector_world() const;
	glm::vec3 getTranslateVector_world() const;
	glm::vec3 getRotateVector_local() const;
	glm::vec3 getScaleVector_local() const;
	glm::vec3 getTranslateVector_local() const;
	glm::vec3 getModelCenter() const;

private:

	void setModelBoxVetrtices();
	// getters
	glm::mat4x4 getRotationMatrix();
	glm::mat4x4 getTranslationMatrix();
	glm::mat4x4 getScalingMatrix();

	glm::mat4x4 getRotationMatrix_local();

	glm::mat4x4 getTranslationMatrix_local();

	glm::mat4x4 getScalingMatrix_local();


	// default memebers
	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::string model_name_;
	
	// matrices
	glm::mat4x4 worldTransform;
	glm::mat4x4 localTransform;
	glm::mat4x4 rotateTransform;
	glm::mat4x4 rotateTransform_local;

	// vecs
	glm::vec3 translateVector;
	glm::vec3 scaleVector;
	glm::vec3 rotateVector;

	glm::vec3 translateVector_local;
	glm::vec3 scaleVector_local;
	glm::vec3 rotateVector_local;

	// model points attributes
	glm::vec3 bottom;
	glm::vec3 top;
	glm::vec3 centerPoint;
	std::map<int, int> verticesFaces;
	box boxAttr;
	bool worldUpdateFlag;
	bool localUpdateFlag;

};
