#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	model_name_(model_name),
	translateVector(glm::vec3(0.0f, 0.0f, 0.0f)),
	scaleVector(glm::vec3(1.0f, 1.0f, 1.0f)),
	rotateVector(glm::vec3(0.0f, 0.0f, 0.0f)),
	buttom(0),
	top(0),

	centerPoint(0)
{
	for (std::vector<glm::vec3>::const_iterator iterator = vertices.cbegin(); iterator != vertices.end(); ++iterator)
	{
		buttom.x = std::min(buttom.x, iterator->x);
		buttom.y = std::min(buttom.y, iterator->y);
		buttom.z = std::min(buttom.z, iterator->z);

		top.y = std::max(top.y, iterator->y);
		top.y = std::max(top.y, iterator->y);
		top.z = std::max(top.z, iterator->z);

		centerPoint.x += iterator->x;
		centerPoint.y += iterator->y;
		centerPoint.z += iterator->z;
	}

	centerPoint.x = centerPoint.x / vertices.size();
	centerPoint.y = centerPoint.y / vertices.size();
	centerPoint.z = centerPoint.z / vertices.size();

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

glm::mat4x4 MeshModel::getRotationMatrix()
{
	float angle;
	float pi = atan(1) * 4;
	glm::mat4x4 rotateTransform(1);	
	glm::mat4x4 translateToCenter(1);

	translateToCenter[3].x = -centerPoint.x;
	translateToCenter[3].y = -centerPoint.y;
	translateToCenter[3].z = -centerPoint.z;

	if (rotateVector.x != 0) {
		angle = rotateVector.x * pi /180;
		rotateTransform[0] = glm::vec4(1, 0, 0, 0);
		rotateTransform[1] = glm::vec4(0, cos(angle), -sin(angle), 0);
		rotateTransform[2] = glm::vec4(0, sin(angle), cos(angle), 0);
		rotateTransform[3] = glm::vec4(0, 0, 0, 1);
	}
	else if (rotateVector.y != 0) {
		angle = rotateVector.y * pi / 180;
		rotateTransform[0] = glm::vec4(cos(angle), 0, sin(angle), 0);
		rotateTransform[1] = glm::vec4(0, 1, 0, 0);
		rotateTransform[2] = glm::vec4(-sin(angle),0, cos(angle), 0);
		rotateTransform[3] = glm::vec4(0, 0, 0, 1);
	}
	else if (rotateVector.z != 0) {
		angle = rotateVector.z * pi / 180;
		rotateTransform[0] = glm::vec4(cos(angle), -sin(angle), 0, 0);
		rotateTransform[1] = glm::vec4(sin(angle), cos(angle), 0, 0);
		rotateTransform[2] = glm::vec4(0, 0, 1, 0);
		rotateTransform[3] = glm::vec4(0, 0, 0, 1);
	}


	return glm::inverse(translateToCenter) * rotateTransform * translateToCenter;
}

glm::mat4x4 MeshModel::getTranslationMatrix()
{
	glm::mat4x4 transmat(1);

	transmat[3].x = translateVector.x;
	transmat[3].y = translateVector.y;
	transmat[3].z = translateVector.z;
	
	return transmat;
}

glm::mat4x4 MeshModel::getScalingMatrix()
{
	glm::mat4x4 scaleMat(1);
	scaleMat[0].x *= scaleVector.x;
	scaleMat[1].y *= scaleVector.y;
	scaleMat[2].z *= scaleVector.z;

	return scaleMat;
}

const glm::mat4x4 MeshModel::getWorldTransformation()
{
	glm::mat4x4 worldTransform(1);
	glm::mat4x4 translate = getTranslationMatrix();
	glm::mat4x4 rotate = getRotationMatrix();
	rotateVector = glm::vec3(0.0f);
	glm::mat4x4 scale = getScalingMatrix();
	worldTransform = translate * rotate * scale;
	return worldTransform;
}

std::vector<glm::vec3> MeshModel::getNormals() const
{
	return normals_;
}

void MeshModel::setScale(glm::vec3 newScale)
{
	scaleVector = glm::vec3(newScale.x, newScale.y, newScale.z);
}

void MeshModel::setRotate(glm::vec3 newRotate)
{
	rotateVector = newRotate;
}

void MeshModel::setTranslate(glm::vec3 newTranslate)
{
	translateVector = newTranslate;
}


void MeshModel::setFaceNormals()
{
	std::map<int, std::vector<glm::vec3>> forCalculate;
	const int facesSize = faces_.size();

	for (int i = 0; i < facesSize; i++)
	{
		Face currFace = faces_[i];

		int v1 = currFace.GetVertexIndex(0) - 1;
		float x1 = vertices_[v1].x, y1 = vertices_[v1].y, z1 = vertices_[v1].z;
		int v2 = currFace.GetVertexIndex(1) - 1;
		float x2 = vertices_[v2].x, y2 = vertices_[v2].y, z2 = vertices_[v2].z;
		int v3 = currFace.GetVertexIndex(2) - 1;
		float x3 = vertices_[v3].x, y3 = vertices_[v3].y, z3 = vertices_[v3].z;
		

		glm::vec4 originalPoint1 = glm::vec4(x1, y1, z1, 1.0f);
		glm::vec4 originalPoint2 = glm::vec4(x2, y2, z2, 1.0f);
		glm::vec4 originalPoint3 = glm::vec4(x3, y3, z3, 1.0f);

		glm::vec4 vector1 = (originalPoint2 - originalPoint1);
		glm::vec4 vector2 = (originalPoint3 - originalPoint1);
		glm::vec4 vector3 = (originalPoint3 - originalPoint2);

		glm::vec3 a = glm::vec3(vector1.x, vector1.y, vector1.z);
		glm::vec3 b = glm::vec3(vector2.x, vector2.y, vector2.z);
		glm::vec3 c = glm::vec3(vector3.x, vector3.y, vector3.z);

		glm::vec3 normalHelper1 = glm::cross(a, b);
		glm::vec3 normalHelper2 = glm::cross(-a, c);
		glm::vec3 normalHelper3 = glm::cross(-b, -c);
		glm::vec3 _normalPoint = (normalHelper1 + normalHelper2 + normalHelper3) / 3.0f;

		faces_[i].setNormal(_normalPoint);

		forCalculate[v1].push_back(_normalPoint);
		forCalculate[v2].push_back(_normalPoint);
		forCalculate[v3].push_back(_normalPoint);

		if (verticesFaces.find(v1) == verticesFaces.end())
		{
			verticesFaces[v1] = 0;
		}
		
		verticesFaces[v1] = verticesFaces.at(v1) + 1;

		if (verticesFaces.find(v2) == verticesFaces.end())
		{
			verticesFaces[v2] = 0;
		}

		verticesFaces[v2] = verticesFaces.at(v2) + 1;

		if (verticesFaces.find(v3) == verticesFaces.end())
		{
			verticesFaces[v3] = 0;
		}

		verticesFaces[v3] = verticesFaces.at(v3) + 1;
	}


}

const int MeshModel::getVertexFacesSum(int indx) const {
	return verticesFaces.at(indx);
}

