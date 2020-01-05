#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>
#include <array>
#include <queue>
#include <vector>

class Renderer
{
	struct faceZDist {
		int faceIndex;
		float distance;

		bool operator > (const faceZDist other) const
		{
			return (distance > other.distance);
		}
	};


public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(const Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void SetViewport(int width, int height);
	void DrawAsterisk();
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	void drawAxis(const glm::mat4 & projectionMatrix, const glm::mat4 & viewMatrix);
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
	void PutPixel(const int i, const int j, const glm::vec3& color);
	glm::vec3 GetPixel(int i, int j);
	void SetViewportHeight(const int _viewport_height);
	void SetViewportWidth(const int _viewport_width);

private:
	void CreateBuffers(int w, int h);
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();
	float* color_buffer_;
	int viewport_width_;
	int viewport_height_;
	GLuint gl_screen_tex_;
	GLuint gl_screen_vtc_;
	glm::vec4 trasformVec3(const glm::mat4 & transformationMatrix, glm::vec3 vector);

	void drawFacesNormals(const glm::vec3 & vec1, const glm::vec3 & vec2, const glm::vec3 & vec3, const glm::mat4x4 & transformationMatrix, const Face & currFace);
	void drawVerticesNormals(const MeshModel & mesh, const std::map<int, std::vector<int>>& verticesNormals, const std::vector<glm::vec3>& vertices, const glm::mat4x4 & transformationMatrix);
	void drawBoundBox(const MeshModel mesh, const glm::mat4x4 & transformationMatrix);
	void flood(int x, int y, const glm::vec3& new_col, const glm::vec3& sidesColor, const glm::vec4& zAx);
	void drawFaceTriangle(const glm::vec3 & vec1, const glm::vec3 & vec2, const glm::vec3 & vec3, const glm::mat4x4 & transformationMatrix, const Face & currFace);
};
