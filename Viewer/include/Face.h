#pragma once
#include "vector"
#include <string>
#include <glm/glm.hpp>

class Face
{
public:
	Face(std::istream& issLine);
	int GetVertexIndex(int index) const;
	int GetNormalIndex(int index) const;
	int GetTextureIndex(int index) const;

	int getVerticesCount() const;
	void setNormal(glm::vec3 _normal);
	glm::vec3 getNormal() const;

private:
	std::vector<int> vertex_indices_;
	std::vector<int> normal_indices_;
	std::vector<int> texture_indices_;

	glm::vec3 normal;
};