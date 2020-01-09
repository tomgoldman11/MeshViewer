#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Utils.h"

glm::vec4 Utils::Vec4FromVec3(const glm::vec3& vector3) 
{
	glm::vec4 vector4;
	vector4.x = vector3.x;
	vector4.y = vector3.y;
	vector4.z = vector3.z;
	vector4.w = 1;
	
	return vector4;

}

 glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

std::shared_ptr<MeshModel> Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::ifstream ifile(filePath.c_str());
	std::map<int, std::vector<int>> verticesFacesNormals;

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			vertices.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vn")
		{
			normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			// TODO: Handle texture coordinates
		}
		else if (lineType == "f")
		{
			Face currFace(issLine);
			faces.push_back(currFace);
			verticesFacesNormals[currFace.GetVertexIndex(0) - 1].insert(verticesFacesNormals[currFace.GetVertexIndex(0) - 1].begin(), currFace.GetNormalIndex(0) - 1);
			verticesFacesNormals[currFace.GetVertexIndex(1) - 1].insert(verticesFacesNormals[currFace.GetVertexIndex(1) - 1].begin(), currFace.GetNormalIndex(1) - 1);
			verticesFacesNormals[currFace.GetVertexIndex(2) - 1].insert(verticesFacesNormals[currFace.GetVertexIndex(2) - 1].begin(), currFace.GetNormalIndex(2) - 1);
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}
	std::vector<glm::vec3> verticesNormals(vertices.size());

	for (std::map<int, std::vector<int>>::const_iterator vInd = verticesFacesNormals.begin(); vInd != verticesFacesNormals.end(); vInd++) {
		glm::vec3 sumNormals(0);
		std::vector<int> listNormals = vInd->second;
		unsigned int i = 0;
		for (i; i < listNormals.size(); ++i)
		{
			sumNormals += normals[listNormals[i]];
		}
		sumNormals = sumNormals / (float)listNormals.size();
		
		try
		{
			verticesNormals[vInd->first] = sumNormals;
		}
		catch (int e)
		{
			verticesNormals[0] = sumNormals;
		}
	}


	return std::make_shared<MeshModel>(faces, vertices, normals, Utils::GetFileName(filePath), verticesNormals);
}


std::shared_ptr<Camera> Utils::LoadCamera(const glm::vec3 eye, const glm::vec3 at, const glm::vec3 up) {
	glm::vec3 eye_ = eye;
	glm::vec3 at_ = at;
	glm::vec3 up_ = up;
	return std::make_shared<Camera>(eye_, at_, up_);
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}

float Utils::sign(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}


bool Utils::PointInTriangle(glm::vec2 pt, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, float * zPoint)
{
	//float d1, d2, d3;
	//bool has_neg, has_pos;

	//d1 = sign(pt, v1, v2);
	//d2 = sign(pt, v2, v3);
	//d3 = sign(pt, v3, v1);

	//has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	//has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	//return !(has_neg && has_pos);

	float a = 0.0f, b = 0.0f, c = 0.0f;
	bool value = getLinearInterpolationOfPoints(pt.x, pt.y, v1, v2, v3, &a, &b, &c, nullptr);
	if (value == false) return false;
	float z1 = v1.z, z2 = v2.z, z3 = v3.z;
	if (zPoint != nullptr) {
		*zPoint = a * z1 + b * z2 + c * z3;
	}
	return true;

}

bool Utils::getLinearInterpolationOfPoints(float x, float y, const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, float* const alpha, float* const beta, float* const gama, bool* const changed)
{
	if (changed != nullptr) {
		*changed = false;
	}
	//source: http://totologic.blogspot.com/2014/01/accurate-point-in-triangle-test.html
	float x1 = point1.x, x2 = point2.x, x3 = point3.x;
	float y1 = point1.y, y2 = point2.y, y3 = point3.y;
	float z1 = point1.z, z2 = point2.z, z3 = point3.z;
	float diva = ((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
	if (diva == 0) return false;
	float a = ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3)) / diva;
	if (a < 0.0f || a > 1.0f) return false;
	float divb = ((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
	if (divb == 0) return false;
	float b = ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3)) / divb;
	if (b < 0.0f || b > 1.0f) return false;
	float c = 1.0f - a - b;
	if (c < 0.0f || c > 1.0f) return false;
	if (alpha == nullptr || beta == nullptr || gama == nullptr) {
		//do nothing
	}
	else {
		*alpha = a;
		*beta = b;
		*gama = c;
		if (changed != nullptr) {
			*changed = true;
		}
	}
	return true;
}