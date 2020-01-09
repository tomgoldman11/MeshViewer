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
	//struct faceZDist {
	//	int faceIndex;
	//	float distance;

	//	bool operator > (const faceZDist other) const
	//	{
	//		return (distance > other.distance);
	//	}
	//	bool operator < (const faceZDist other) const
	//	{
	//		return (distance < other.distance);
	//	}
	//};

	struct zColor {
		float z;
		glm::vec3 color;
	};

	struct triangleVecs {
		glm::vec3 & vec1P;
		glm::vec3 & vec2P;
		glm::vec3 & vec3P;
	};

	struct triangleVecsNormals {
		glm::vec3 & vec1N;
		glm::vec3 & vec2N;
		glm::vec3 & vec3N;
	};

public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(const Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void SetViewport(int width, int height);
	float getZOnLine(int x, int y, int x1, int y1, float z1, int x2, int y2, float z2);
	//void DrawAsterisk();
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	void drawAxis(const glm::mat4 & projectionMatrix, const glm::mat4 & viewMatrix);
	void DrawLine(const glm::ivec3& p1, const glm::ivec3& p2, const glm::vec3& color);
	void ALTER_DrawLine(const glm::ivec3 & p1, const glm::ivec3 & p2, const glm::vec3 & color);
	void PutPixel(const int i, const int j, const glm::vec3& color);
	glm::vec3 GetPixel(int i, int j);
	void SetViewportHeight(const int _viewport_height);
	void SetViewportWidth(const int _viewport_width);

private:
	std::map<std::pair<int, int>, zColor> Mapix;
	void CreateBuffers(int w, int h);
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();
	float* color_buffer_;
	int viewport_width_;
	int viewport_height_;
	GLuint gl_screen_tex_;
	GLuint gl_screen_vtc_;
	glm::vec4 trasformVec3(const glm::mat4 & transformationMatrix, glm::vec3 vector);
	glm::vec4 eyePoint;

	glm::vec3 drawFacesNormals(const glm::vec3 & vec1, const glm::vec3 & vec2, const glm::vec3 & vec3, const glm::mat4x4 & transformationMatrix, const Face & currFace, const bool print_normals);
	void drawVerticesNormals(const MeshModel & mesh, const std::vector<glm::vec3>& vertices, const glm::mat4x4 & transformationMatrix, const bool print_normals);
	void drawBoundBox(const MeshModel mesh, const glm::mat4x4 & transformationMatrix);
	void flood(int x, int y, const glm::vec3& new_col, const glm::vec3& sidesColor, Face & currFace);
	void drawFaceTriangle(const glm::vec3 & vec1, const glm::vec3 & vec2, const glm::vec3 & vec3, const glm::mat4x4 & transformationMatrix, const Face & currFace, const glm::vec3 &color);
	void addToMapix(int x, int y, float z, const glm::vec3& color, bool addColor=false);
	void addColor(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& color, const glm::mat4x4 & transformationMatrix);
	glm::vec3 getFaceChanger(const glm::mat4x4 & globalTransformationMatrix, const LightSource & light, const glm::vec3 & normalTEST, const triangleVecs & currTriangle, const material & _materialAttr, const glm::vec3 & point);
	glm::vec3 generateNewColor(const glm::mat4x4 & globalTransformationMatrix, const std::vector<std::shared_ptr<LightSource>> & lights_sources, const glm::mat4x4 & transformationMatrix, const glm::vec3 & normalTEST, const glm::vec3 & eye, const triangleVecs & currTriangle, const material & _materialAttr);
	glm::vec3 drawVertixNormal( const glm::vec3 & vertex, const glm::vec3 & normal, const glm::mat4x4 & transformationMatrix, const bool print_normals);
	glm::vec3 handleLight(const Shading & shadingType, const glm::mat4x4 & helper, const LightSource & currentLight, const glm::vec3 &faceNormal, const triangleVecs & origTriangle, const material & meshMaterialAttr, const triangleVecsNormals & origVerticesNormals, const glm::mat4x4 & transformationMatrix, const bool & printVerticesNormals, const glm::vec3 & faceCenter);
};
