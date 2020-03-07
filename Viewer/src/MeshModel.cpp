#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name, std::vector<glm::vec3> _verticesNormals) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	model_name_(model_name),
	translateVector(glm::vec3(0.0f, 0.0f, 0.0f)),
	scaleVector(glm::vec3(1.0f, 1.0f, 1.0f)),
	rotateVector(glm::vec3(0.0f, 0.0f, 0.0f)), 
	translateVector_local(glm::vec3(0.0f, 0.0f, 0.0f)),
	scaleVector_local(glm::vec3(1.0f, 1.0f, 1.0f)),
	rotateVector_local(glm::vec3(0.0f, 0.0f, 0.0f)),
	bottom(2000),
	top(0),
	centerPoint(0),
	worldUpdateFlag(true),
	localUpdateFlag(true),
	sidesColor(glm::vec3(0.0f,0.0f,0.0f)),
	objColor(glm::vec3(0.0f, 0.0f, 255.0f)), 
	objMaterial({ glm::vec3(1.0f, 0.5f, 0.31f),glm::vec3(1.0f, 0.5f, 0.31f),glm::vec3(0.5f, 0.5f, 0.5f), 32 }), // ambient<K,L>, diffuse<K,L>, specular <K,L> , shininess <int>
	verticesNormals(_verticesNormals),
	textureType(uniform) //uniform, non_uniform, random_uniform
{
	//for (std::vector<glm::vec3>::const_iterator iterator = vertices.cbegin(); iterator != vertices.end(); ++iterator)
	//{
	//	bottom.x = std::min(bottom.x, iterator->x);
	//	bottom.y = std::min(bottom.y, iterator->y);
	//	bottom.z = std::min(bottom.z, iterator->z);

	//	top.x = std::max(top.x, iterator->x);
	//	top.y = std::max(top.y, iterator->y);
	//	top.z = std::max(top.z, iterator->z);

	//	//centerPoint.x += iterator->x;
	//	//centerPoint.y += iterator->y;
	//	//centerPoint.z += iterator->z;
	//}

	////centerPoint.x = centerPoint.x / vertices.size();
	////centerPoint.y = centerPoint.y / vertices.size();
	////centerPoint.z = centerPoint.z / vertices.size();

	//centerPoint.x = (top.x +bottom.x)/2;
	//centerPoint.y = (top.y + bottom.y) / 2;
	//centerPoint.z = (top.z + bottom.z) / 2;

	//setModelBoxVetrtices();

	modelVertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;

			Vertex vertex;
			vertex.position = vertices[vertexIndex];
			vertex.normal = normals[vertexIndex];

			if (textureCoords.size() > 0)
			{
				int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
				vertex.textureCoords = textureCoords[textureCoordsIndex];
			}

			modelVertices.push_back(vertex);
		}
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);

}

MeshModel::~MeshModel()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

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
	float pi = float(atan(1) * 4);
	glm::mat4x4 rotateTransformX(1);
	glm::mat4x4 rotateTransformY(1);
	glm::mat4x4 rotateTransformZ(1);

	if (rotateVector.x != 0) {
		angle = rotateVector.x * pi /180;
		rotateTransformX[0] = glm::vec4(1, 0, 0, 0);
		rotateTransformX[1] = glm::vec4(0, cos(angle), -sin(angle), 0);
		rotateTransformX[2] = glm::vec4(0, sin(angle), cos(angle), 0);
		rotateTransformX[3] = glm::vec4(0, 0, 0, 1);
	}
	if (rotateVector.y != 0) {
		angle = rotateVector.y * pi / 180;
		rotateTransformY[0] = glm::vec4(cos(angle), 0, sin(angle), 0);
		rotateTransformY[1] = glm::vec4(0, 1, 0, 0);
		rotateTransformY[2] = glm::vec4(-sin(angle),0, cos(angle), 0);
		rotateTransformY[3] = glm::vec4(0, 0, 0, 1);
	}
	if (rotateVector.z != 0) {
		angle = rotateVector.z * pi / 180;
		rotateTransformZ[0] = glm::vec4(cos(angle), -sin(angle), 0, 0);
		rotateTransformZ[1] = glm::vec4(sin(angle), cos(angle), 0, 0);
		rotateTransformZ[2] = glm::vec4(0, 0, 1, 0);
		rotateTransformZ[3] = glm::vec4(0, 0, 0, 1);
	}

	rotateTransform = rotateTransformX * rotateTransformY * rotateTransformZ;
	return rotateTransform ;
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

glm::mat4x4 MeshModel::getRotationMatrix_local()
{
	float angle;
	float pi = float(atan(1) * 4);
	glm::mat4x4 rotateTransformX(1);
	glm::mat4x4 rotateTransformY(1);
	glm::mat4x4 rotateTransformZ(1);
	glm::mat4x4 translateToCenter(1);
	glm::mat4x4 NEGtranslateToCenter(1);

	translateToCenter[3].x = centerPoint.x;
	translateToCenter[3].y = centerPoint.y;
	translateToCenter[3].z = centerPoint.z;

	NEGtranslateToCenter[3].x = -centerPoint.x;
	NEGtranslateToCenter[3].y = -centerPoint.y;
	NEGtranslateToCenter[3].z = -centerPoint.z;

	if (rotateVector_local.x != 0) {
		angle = rotateVector_local.x * pi / 180;
		rotateTransformX[0] = glm::vec4(1, 0, 0, 0);
		rotateTransformX[1] = glm::vec4(0, cos(angle), -sin(angle), 0);
		rotateTransformX[2] = glm::vec4(0, sin(angle), cos(angle), 0);
		rotateTransformX[3] = glm::vec4(0, 0, 0, 1);
	}
	if (rotateVector_local.y != 0) {
		angle = rotateVector_local.y * pi / 180;
		rotateTransformY[0] = glm::vec4(cos(angle), 0, sin(angle), 0);
		rotateTransformY[1] = glm::vec4(0, 1, 0, 0);
		rotateTransformY[2] = glm::vec4(-sin(angle), 0, cos(angle), 0);
		rotateTransformY[3] = glm::vec4(0, 0, 0, 1);
	}
	if (rotateVector_local.z != 0) {
		angle = rotateVector_local.z * pi / 180;
		rotateTransformZ[0] = glm::vec4(cos(angle), -sin(angle), 0, 0);
		rotateTransformZ[1] = glm::vec4(sin(angle), cos(angle), 0, 0);
		rotateTransformZ[2] = glm::vec4(0, 0, 1, 0);
		rotateTransformZ[3] = glm::vec4(0, 0, 0, 1);
	}

	rotateTransform_local = rotateTransformX * rotateTransformY * rotateTransformZ;
	return translateToCenter * rotateTransform_local * NEGtranslateToCenter;
}

glm::mat4x4 MeshModel::getTranslationMatrix_local()
{
	glm::mat4x4 transmat(1);
	glm::mat4x4 translateToCenter(1);
	glm::mat4x4 NEGtranslateToCenter(1);

	translateToCenter[3].x = centerPoint.x;
	translateToCenter[3].y = centerPoint.y;
	translateToCenter[3].z = centerPoint.z;

	NEGtranslateToCenter[3].x = -centerPoint.x;
	NEGtranslateToCenter[3].y = -centerPoint.y;
	NEGtranslateToCenter[3].z = -centerPoint.z;

	transmat[3].x = translateVector_local.x;
	transmat[3].y = translateVector_local.y;
	transmat[3].z = translateVector_local.z;

	return translateToCenter * transmat * NEGtranslateToCenter;
}

glm::mat4x4 MeshModel::getScalingMatrix_local()
{
	glm::mat4x4 scaleMat(1);
	glm::mat4x4 translateToCenter(1);
	glm::mat4x4 NEGtranslateToCenter(1);

	translateToCenter[3].x = centerPoint.x;
	translateToCenter[3].y = centerPoint.y;
	translateToCenter[3].z = centerPoint.z;

	NEGtranslateToCenter[3].x = -centerPoint.x;
	NEGtranslateToCenter[3].y = -centerPoint.y;
	NEGtranslateToCenter[3].z = -centerPoint.z;

	scaleMat[0].x *= scaleVector_local.x;
	scaleMat[1].y *= scaleVector_local.y;
	scaleMat[2].z *= scaleVector_local.z;

	return translateToCenter * scaleMat * NEGtranslateToCenter;
}

const glm::mat4x4 MeshModel::getWorldTransformation()
{
		glm::mat4x4 translate = getTranslationMatrix();
		glm::mat4x4 rotate = getRotationMatrix();
		rotateVector = glm::vec3(0.0f);
		glm::mat4x4 scale = getScalingMatrix();
		worldTransform = translate * rotate * scale;
		worldUpdateFlag = false;
	return worldTransform;
}

const glm::mat4x4 MeshModel::getLocalTransformation()
{
		glm::mat4x4 translate = getTranslationMatrix_local();
		glm::mat4x4 rotate = getRotationMatrix_local();
		rotateVector_local = glm::vec3(0.0f);
		glm::mat4x4 scale = getScalingMatrix_local();
		localTransform = translate * rotate * scale;
		localUpdateFlag = false;
	return localTransform;
}

std::vector<glm::vec3> MeshModel::getNormals() const
{
	return normals_;
}

void MeshModel::setScale(const float newScale)
{
	setScale(glm::vec3(newScale, newScale, newScale));
}

void MeshModel::setScale(const glm::vec3 newScale)
{
	scaleVector = glm::vec3(newScale.x, newScale.y, newScale.z);
}

void MeshModel::setRotate(const glm::vec3 newRotate)
{
	rotateVector = newRotate;
}

void MeshModel::setTranslate(const glm::vec3 newTranslate)
{
	translateVector = newTranslate;
}

void MeshModel::setScale_local(const float newScale)
{
	setScale_local(glm::vec3(newScale, newScale, newScale));
}

void MeshModel::setScale_local(const glm::vec3 newScale)
{
	scaleVector_local = glm::vec3(newScale.x, newScale.y, newScale.z);
}

void MeshModel::setRotate_local(const glm::vec3 newRotate)
{
	rotateVector_local = newRotate;
}

void MeshModel::setTranslate_local(const glm::vec3 newTranslate)
{
	translateVector_local = newTranslate;
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

	}


}

void MeshModel::setSidesColor(const glm::vec3 & _color)
{
	sidesColor.x = (_color.x > 0.0f) ? _color.x : 0.0f;
	sidesColor.y = (_color.y > 0.0f) ? _color.y : 0.0f;
	sidesColor.z = (_color.z > 0.0f) ? _color.z : 0.0f;
}

void MeshModel::setObjColor(const glm::vec3 & _color)
{
	objColor.x = (_color.x > 0.0f) ? _color.x : 0.0f;
	objColor.y = (_color.y > 0.0f) ? _color.y : 0.0f;
	objColor.z = (_color.z > 0.0f) ? _color.z : 0.0f;

}

void MeshModel::setDiffuse(const glm::vec3 & _diffuse)
{
	objMaterial.diffuse = _diffuse;
}

void MeshModel::setSpecular(const glm::vec3 & _specular)
{
	objMaterial.specular = _specular;
}

void MeshModel::setAmbient(const glm::vec3 & _ambient)
{
	objMaterial.ambient = _ambient;
}

void MeshModel::setShininess(const int _shininess)
{
	objMaterial.shininess = _shininess;
}

void MeshModel::setTextureType(const texture & _type)
{
	textureType = _type;
}


glm::vec3 MeshModel::getRotateVector_world() const
{
	return rotateVector;
}

glm::vec3 MeshModel::getScaleVector_world() const
{
	return scaleVector;
}

glm::vec3 MeshModel::getTranslateVector_world() const
{
	return translateVector;
}

glm::vec3 MeshModel::getRotateVector_local() const
{
	return rotateVector_local;
}

glm::vec3 MeshModel::getScaleVector_local() const
{
	return scaleVector_local;
}

glm::vec3 MeshModel::getTranslateVector_local() const
{
	return translateVector_local;
}

glm::vec3 MeshModel::getModelCenter() const
{
	return centerPoint;
}

glm::vec3 MeshModel::getSidesColor() const
{
	return sidesColor;
}

glm::vec3 MeshModel::getObjColor() const
{
	return objColor;
}

glm::vec3 MeshModel::getAmbient() const
{
	return objMaterial.ambient;
}

glm::vec3 MeshModel::getDiffuse() const
{
	return  objMaterial.diffuse;
}

glm::vec3 MeshModel::getSpecular() const
{
	return objMaterial.specular;
}

int MeshModel::getShininess() const
{
	return objMaterial.shininess;
}

material MeshModel::getObjMaterialStruct() const
{
	return objMaterial;
}

glm::vec3 MeshModel::getVertexNormal(int index) const
{
	if (index < 0 || index >= verticesNormals.size()) {
		return glm::vec3(0, 0, 0);
	}
	return verticesNormals[index];
}

int MeshModel::getVerticesCount() const
{
	return vertices_.size();
}

int MeshModel::getVerticesCount2() const
{
	return modelVertices.size();
}

texture MeshModel::getTextureType() const
{
	return textureType;
}

const int MeshModel::getVertexFacesSum(int indx) const {
	return verticesFaces.at(indx);
}

void MeshModel::getModelBoxVetrtices(glm::vec3 & XnYZ, glm::vec3 & XnYnZ, glm::vec3 & nXnYnZ, glm::vec3 & nXnYZ, glm::vec3 & XYZ, glm::vec3 & XYnZ, glm::vec3 & nXYnZ, glm::vec3 & nXYZ) const
{
	XnYZ = boxAttr.XnYZ;
	XnYnZ = boxAttr.XnYnZ;
	nXnYnZ = boxAttr.nXnYnZ;
	nXnYZ = boxAttr.nXnYZ;
	XYZ = boxAttr.XYZ;
	XYnZ = boxAttr.XYnZ;
	nXYnZ = boxAttr.nXYnZ;
	nXYZ = boxAttr.nXYZ;
}

void MeshModel::setModelBoxVetrtices() 
{
	boxAttr.XnYZ = glm::vec3(top.x, bottom.y, top.z);
	boxAttr.XnYnZ = glm::vec3(top.x, bottom.y, bottom.z);
	boxAttr.nXnYnZ = glm::vec3(bottom.x, bottom.y, bottom.z);
	boxAttr.nXnYZ = glm::vec3(bottom.x, bottom.y, top.z);
	boxAttr.XYZ = glm::vec3(top.x, top.y, top.z);
	boxAttr.XYnZ = glm::vec3(top.x, top.y, bottom.z);
	boxAttr.nXYnZ = glm::vec3(bottom.x, top.y, bottom.z);
	boxAttr.nXYZ = glm::vec3(bottom.x, top.y, top.z);
}

GLuint MeshModel::GetVAO() const
{
	return vao;
}

void MeshModel::BindTextures()
{
	textureModel.bind(0); 
}

void MeshModel::UnbindTextures()
{
	textureModel.unbind(0);
}
