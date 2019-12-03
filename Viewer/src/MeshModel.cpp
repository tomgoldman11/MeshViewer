#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	model_name_(model_name),
	translateVector(glm::vec3(0.0f, 0.0f, 0.0f)),
	scaleVector(glm::vec3(1.0f, 1.0f, 1.0f)),
	rotateVector(0.0f)
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
	return glm::mat4x4(1);
}

const glm::mat4x4 MeshModel::getTranslationMatrix()
{
	glm::mat4x4 transmat(1);

	transmat[0].w = translateVector.x;
	transmat[1].w = translateVector.y;
	transmat[2].w = translateVector.z;
	
	return transmat;
}

const glm::mat4x4 MeshModel::getScalingMatrix()
{
	glm::mat4x4 scalemat(1);
	scalemat[0].x *= scaleVector.x;
	scalemat[1].y *= scaleVector.y;
	scalemat[2].z *= scaleVector.z;
	return scalemat;
}

const glm::mat4x4& MeshModel::getWorldTransformation()
{
	worldTransform = glm::mat4x4(1);
	glm::mat4x4 translate = getTranslationMatrix();
	glm::mat4x4 rotate = getRotationMatrix();
	rotateVector = glm::vec3(0.0f);
	glm::mat4x4 scale = getScalingMatrix();
	//worldTransform = translate * rotate * scale;
	worldTransform = scale * translate * rotate;
	return worldTransform;
}