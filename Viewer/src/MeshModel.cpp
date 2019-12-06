#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	model_name_(model_name),
	translateVector(glm::vec3(0.0f, 0.0f, 0.0f)),
	scaleVector(glm::vec3(1.0f, 1.0f, 1.0f)),
	rotateVector(glm::vec3(30.0f, 0.0f, 0.0f))
{

}

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces_[index];
}

std::vector<glm::vec3> MeshModel::getVertices() const
{
	return vertices_;
}

int MeshModel::GetFacesCount() const
{
	return faces_.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name_;
}

const glm::mat4x4 MeshModel::getRotationMatrix()
{
	float angle;
	if (rotateVector.x != 0) {
		angle = rotateVector.x;
		rotateTransform[0] = glm::vec4(1, 0, 0, 0);
		rotateTransform[1] = glm::vec4(0, cos(-angle), sin(-angle), 0);
		rotateTransform[2] = glm::vec4(0, -sin(-angle), cos(-angle), 0);
		rotateTransform[3] = glm::vec4(0, 0, 0, 1);
		return rotateTransform;
	}
	if (rotateVector.y != 0) {
		angle = rotateVector.y;
		rotateTransform[0] = glm::vec4(cos(angle), 0, -sin(angle), 0);
		rotateTransform[1] = glm::vec4(0, 1, 0, 0);
		rotateTransform[2] = glm::vec4(sin(angle),0, cos(angle), 0);
		rotateTransform[3] = glm::vec4(0, 0, 0, 1);
		return rotateTransform;
	}
	if (rotateVector.z != 0) {
		angle = rotateVector.z;
		rotateTransform[0] = glm::vec4(cos(angle), -sin(angle), 0, 0);
		rotateTransform[1] = glm::vec4(sin(angle), cos(angle), 0, 0);
		rotateTransform[2] = glm::vec4(0, 0, 1, 0);
		rotateTransform[3] = glm::vec4(0, 0, 0, 1);
		return rotateTransform;
	}

	return glm::mat4x4(1);
}

const glm::mat4x4 MeshModel::getTranslationMatrix()
{
	glm::mat4x4 transmat(1);

	transmat[3].x = translateVector.x;
	transmat[3].y = translateVector.y;
	transmat[3].z = translateVector.z;
	
	return transmat;
}

const glm::mat4x4 MeshModel::getScalingMatrix()
{
	glm::mat4x4 scaleMat(1);
	scaleMat[0].x *= scaleVector.x;
	scaleMat[1].y *= scaleVector.y;
	scaleMat[2].z *= scaleVector.z;

	return scaleMat;
}

const glm::mat4x4 MeshModel::getWorldTransformation()
{
	worldTransform = glm::mat4x4(1);
	glm::mat4x4 translate = getTranslationMatrix();
	glm::mat4x4 rotate = getRotationMatrix();
	rotateVector = glm::vec3(0.0f);
	glm::mat4x4 scale = getScalingMatrix();
	worldTransform = translate * rotate * scale;
	return worldTransform;
}

void MeshModel::setScale(float newScale) 
{
	scaleVector = glm::vec3(newScale, newScale, newScale);
}

void MeshModel::setRotate(glm::vec3 newRotate)
{
	rotateVector = newRotate;
}