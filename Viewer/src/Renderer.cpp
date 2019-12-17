#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include "Renderer.h"
#include "InitShader.h"
#include "Utils.h"
#define NORMALS_LENGHTV 30.0
#define NORMALS_LENGHTF 0.2
#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width_(viewport_width),
	viewport_height_(viewport_height)
{
	InitOpenGLRendering();
	CreateBuffers(viewport_width, viewport_height);

	DrawLine(glm::vec2(0, 0), glm::vec2(1000, 1000), glm::vec3(0, 0, 0));
}

Renderer::~Renderer()
{
	delete[] color_buffer_;
}


void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;

	color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
	color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
	color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
}

void Renderer::SetViewportHeight(const int _viewport_height)
{
	viewport_height_ = _viewport_height;
}

void Renderer::SetViewportWidth(const int _viewport_width)
{
	viewport_width_ = _viewport_width;
}

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	float x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
	dx = x2 - x1;
	dy = y2 - y1;
	dx1 = fabs(dx);
	dy1 = fabs(dy);
	px = 2 * dy1 - dx1;
	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1;
			y = y1;
			xe = x2;
		}
		else
		{
			x = x2;
			y = y2;
			xe = x1;
		}
		PutPixel(x, y, color);
		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
			{
				px = px + 2 * dy1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					y = y + 1;
				}
				else
				{
					y = y - 1;
				}
				px = px + 2 * (dy1 - dx1);
			}
			PutPixel(x, y, color);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1;
			y = y1;
			ye = y2;
		}
		else
		{
			x = x2;
			y = y2;
			ye = y1;
		}
		PutPixel(x, y, color);
		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
			{
				py = py + 2 * dx1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					x = x + 1;
				}
				else
				{
					x = x - 1;
				}
				py = py + 2 * (dx1 - dy1);
			}
			PutPixel(x, y, color);
		}
	}
}


void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenGLBuffer(); //Do not remove this line.
	color_buffer_ = new float[3 * w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex_);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc_);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc_);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[] = {
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[] = {
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1 };

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(vtc));

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"), 0);
}

const glm::vec4 Renderer::trasformVec3(const glm::mat4& transformationMatrix, glm::vec3 vector)
{
	int half_width = viewport_width_ / 2;
	int half_height = viewport_height_ / 2;
	const glm::mat4x4 MMM = glm::mat4x4(
		{ 1,0,0,0 },
		{ 0,1,0,0 },
		{ 0,0,1,0 },
		{ half_width,half_height,0,1 }
	);

	glm::vec4 newPoint = transformationMatrix * Utils::Vec4FromVec3(vector);
	newPoint = newPoint / newPoint.w;
	return MMM * newPoint;
}

void Renderer::drawFacesNormals(const glm::vec3& vec1, const glm::vec3& vec2, const glm::vec3& vec3, const glm::mat4x4& transformationMatrix, const Face& currFace)
{
	glm::vec3 newPoint = (vec1 + vec2 + vec3) / 3.0f;

	glm::vec3 newPoint_T = trasformVec3(transformationMatrix, newPoint);

	glm::vec3 _normalPoint = currFace.getNormal();
	glm::vec4 normalPoint = glm::vec4(_normalPoint.x, _normalPoint.y, _normalPoint.z, 0.0f);

	glm::vec3 normal = 0.5f * _normalPoint + newPoint;
	normal = trasformVec3(transformationMatrix, normal);
	DrawLine(glm::vec2(newPoint_T.x, newPoint_T.y), glm::vec2(normal.x, normal.y), glm::vec3(0, 1, 0));

}

void Renderer::drawVerticesNormals(const MeshModel & mesh, const std::map<int, std::vector<int>> & verticesNormals, const std::vector<glm::vec3> & vertices, const glm::mat4x4& transformationMatrix)
{
	std::vector<glm::vec3> normals = mesh.getNormals();
	 std::map<int, std::vector<int>>::const_iterator vInd;

	for (vInd = verticesNormals.begin(); vInd != verticesNormals.end(); vInd++) {
		glm::vec3 sumNormals(0);
		std::vector<int> listNormals = vInd->second;

		for (int i = 0; i < listNormals.size(); ++i)
		{
			sumNormals += normals[listNormals[i]];
		}
		sumNormals = sumNormals / (float)listNormals.size();

		glm::vec3 vVec = vertices[vInd->first];
		glm::vec3 sumNormals3 = 30.0f * sumNormals + vVec;
		vVec = trasformVec3(transformationMatrix, vVec);
		sumNormals3 = trasformVec3(transformationMatrix, sumNormals3);

		DrawLine(glm::vec2(vVec.x, vVec.y), glm::vec2(sumNormals3.x, sumNormals3.y), glm::vec3(0, 1, 0));
	}

}

void Renderer::drawBoundBox(const MeshModel mesh, const glm::mat4x4& transformationMatrix)
{
	glm::vec3 XnYZ, XnYnZ, nXnYnZ, nXnYZ, XYZ, XYnZ, nXYnZ, nXYZ;
	// get corners vectors
	mesh.getModelBoxVetrtices(XnYZ, XnYnZ, nXnYnZ, nXnYZ, XYZ, XYnZ, nXYnZ, nXYZ);

	// transform points
	XnYZ = trasformVec3(transformationMatrix, XnYZ); //1
	XnYnZ = trasformVec3(transformationMatrix, XnYnZ); //2
	nXnYnZ = trasformVec3(transformationMatrix, nXnYnZ); //3
	nXnYZ = trasformVec3(transformationMatrix, nXnYZ); //4
	XYZ = trasformVec3(transformationMatrix, XYZ); //5
	XYnZ = trasformVec3(transformationMatrix, XYnZ); //6
	nXYnZ = trasformVec3(transformationMatrix, nXYnZ); //7
	nXYZ = trasformVec3(transformationMatrix, nXYZ); //8

	// draw
	DrawLine(glm::vec2(XnYZ.x, XnYZ.y), glm::vec2(XYZ.x, XYZ.y), glm::vec3(0, 0, 1));
	DrawLine(glm::vec2(XnYZ.x, XnYZ.y), glm::vec2(XnYnZ.x, XnYnZ.y), glm::vec3(0, 0, 1));
	DrawLine(glm::vec2(XnYZ.x, XnYZ.y), glm::vec2(nXnYZ.x, nXnYZ.y), glm::vec3(0, 0, 1));

	DrawLine(glm::vec2(nXYnZ.x, nXYnZ.y), glm::vec2(nXYZ.x, nXYZ.y), glm::vec3(0, 0, 1));
	DrawLine(glm::vec2(nXYnZ.x, nXYnZ.y), glm::vec2(XYnZ.x, XYnZ.y), glm::vec3(0, 0, 1));
	DrawLine(glm::vec2(nXYnZ.x, nXYnZ.y), glm::vec2(nXnYnZ.x, nXnYnZ.y), glm::vec3(0, 0, 1));

	DrawLine(glm::vec2(XnYnZ.x, XnYnZ.y), glm::vec2(nXnYnZ.x, nXnYnZ.y), glm::vec3(0, 0, 1));
	DrawLine(glm::vec2(XnYnZ.x, XnYnZ.y), glm::vec2(XYnZ.x, XYnZ.y), glm::vec3(0, 0, 1));

	DrawLine(glm::vec2(nXYZ.x, nXYZ.y), glm::vec2(nXnYZ.x, nXnYZ.y), glm::vec3(0, 0, 1));
	DrawLine(glm::vec2(nXYZ.x, nXYZ.y), glm::vec2(XYZ.x, XYZ.y), glm::vec3(0, 0, 1));

	DrawLine(glm::vec2(nXnYnZ.x, nXnYnZ.y), glm::vec2(nXnYZ.x, nXnYZ.y), glm::vec3(0, 0, 1));
	DrawLine(glm::vec2(XYnZ.x, XYnZ.y), glm::vec2(XYZ.x, XYZ.y), glm::vec3(0, 0, 1));

}

void Renderer::drawFaceTriangle(const glm::vec3 & vec1, const glm::vec3 & vec2, const glm::vec3 & vec3, const glm::mat4x4 & transformationMatrix, const Face & currFace)
{
	glm::vec3 vec1T = trasformVec3(transformationMatrix, vec1);
	glm::vec3 vec2T = trasformVec3(transformationMatrix, vec2);
	glm::vec3 vec3T = trasformVec3(transformationMatrix, vec3);

	// draw the triangle
	if (currFace.getVerticesCount() == 3) {
		DrawLine(glm::vec2(vec1T.x, vec1T.y), glm::vec2(vec2T.x, vec2T.y), glm::vec3(0, 0, 0));
		DrawLine(glm::vec2(vec2T.x, vec2T.y), glm::vec2(vec3T.x, vec3T.y), glm::vec3(0, 0, 0));
		DrawLine(glm::vec2(vec3T.x, vec3T.y), glm::vec2(vec1T.x, vec1T.y), glm::vec3(0, 0, 0));
	}

}

void Renderer::CreateOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width_, viewport_height_, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width_, viewport_height_);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width_, viewport_height_, GL_RGB, GL_FLOAT, color_buffer_);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc_);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			PutPixel(i, j, color);
		}
	}
}

void Renderer::Render(const Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width_ / 2;
	int half_height = viewport_height_ / 2;
	std::map<int, std::vector<int>> verticesNormals;
	glm::mat4 transformationMatrix;
	if (scene.GetActiveCameraIndex() != 0)
	{
		int x;
	}
	const auto& activeCamera = scene.GetActiveCamera(); // getting the active camera in the current scene

	const glm::mat4x4 viewMatrix = activeCamera.getViewTransformation();
    const glm::mat4x4 projectionMatrix = activeCamera.getProjectionTransformation();
	const glm::mat4x4 MMM = glm::mat4x4(
		{1,0,0,0},
		{ 0,1,0,0 },
		{ 0,0,1,0 },
		{ half_width,half_height,0,1 }
	);
	drawAxis(projectionMatrix, viewMatrix);

	for (int i = 0; i < scene.GetModelCount(); i++) {
		MeshModel mesh = scene.GetModel(i);

		// init face normals
		mesh.setFaceNormals();

		//get the vertices
		std::vector<glm::vec3> vertices = mesh.getVertices();

		//get the M matrix (world frame) related to the mesh model
		const glm::mat4x4 modelMatrix = mesh.getLocalTransformation();
		const glm::mat4x4 worldlMatrix = mesh.getWorldTransformation();

		//set a 4X4 transform matrix for the faces T = P*V*M
		transformationMatrix = projectionMatrix* viewMatrix * worldlMatrix * modelMatrix;
		//draw every face
		for (int j = 0; j < mesh.GetFacesCount(); j++) {
			Face currFace = mesh.GetFace(j);
			int v1 = currFace.GetVertexIndex(0) - 1;
			glm::vec3 vec1 = vertices[v1];
			int v2 = currFace.GetVertexIndex(1) - 1;
			glm::vec3 vec2 = vertices[v2];
			int v3 = currFace.GetVertexIndex(2) - 1;
			glm::vec3 vec3 = vertices[v3];
			drawFaceTriangle(vec1, vec2, vec3, transformationMatrix, currFace);

			//draw faces normals
			if (scene.getFacesNormalsStatus()) {
				drawFacesNormals(vec1, vec2, vec3, transformationMatrix, currFace);
			}
			verticesNormals[v1].insert(verticesNormals[v1].begin(), currFace.GetNormalIndex(0)-1);
			verticesNormals[v2].insert (verticesNormals[v2].begin(), currFace.GetNormalIndex(1)-1);
			verticesNormals[v3].insert(verticesNormals[v3].begin(), currFace.GetNormalIndex(2)-1);
		}

		//draw vertices normals
		if (scene.getVerticesNormalsStatus())
		{
			drawVerticesNormals(mesh, verticesNormals, vertices, transformationMatrix);
		}

		// draw bound box
		if (scene.getBoundBoxStatus())
		{
			drawBoundBox(mesh, transformationMatrix);
		}
	}

	for (int i = 0;  i < scene.GetCameraCount(); ++i)
	{
		if (i == scene.GetActiveCameraIndex())
		{
			continue;
		}
		Camera&  cameraObj = scene.GetCamera(i);
		std::shared_ptr<MeshModel> cameraModel = Utils::LoadMeshModel("D:\\graphics proj\\new\\mesh-viewer-tom-tal\\Data\\camera.obj");
		//get the vertices
		std::vector<glm::vec3> vertices = cameraModel->getVertices();
		cameraModel->setTranslate_local(cameraObj.getEye());


		//get the M matrix (world frame) related to the mesh model
		const glm::mat4x4 modelMatrix = cameraModel->getLocalTransformation();
		const glm::mat4x4 worldlMatrix = cameraModel->getWorldTransformation();
		const glm::mat4 inverseLookAt = glm::inverse(cameraObj.getViewTransformation());

		//set a 4X4 transform matrix for the faces T = P*V*M
		transformationMatrix = projectionMatrix* viewMatrix * inverseLookAt;

		for (int j = 0; j < cameraModel->GetFacesCount(); j++) {
			Face currFace = cameraModel->GetFace(j);
			int v1 = currFace.GetVertexIndex(0) - 1;
			glm::vec3 vec1 = vertices[v1];
			int v2 = currFace.GetVertexIndex(1) - 1;
			glm::vec3 vec2 = vertices[v2];
			int v3 = currFace.GetVertexIndex(2) - 1;
			glm::vec3 vec3 = vertices[v3];
			drawFaceTriangle(vec1, vec2, vec3, transformationMatrix, currFace);
		}
	}

}

void Renderer::DrawAsterisk() // testing function for DrawLine
{
	DrawLine(glm::ivec2({ 350,350 }), glm::ivec2({ 350,50 }), glm::vec3(0, 1, 1));
	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 200,50 }), glm::vec3(1, 0, 0));
	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 250,50 }), glm::vec3(0, 0, 1));
	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 350,50 }), glm::vec3(1, 1, 0));
	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 350,150 }), glm::vec3(0, 0, 1));
	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 350,200 }), glm::vec3(1, 0, 0));
	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 350,250 }), glm::vec3(0, 0, 1));
	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 350,350 }), glm::vec3(1, 1, 0));
	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 250,350 }), glm::vec3(0, 0, 1));
	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 200,350 }), glm::vec3(1, 0, 0));
	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 150,350 }), glm::vec3(0, 0, 1));
	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 50,350 }), glm::vec3(1, 1, 0));
	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 50,250 }), glm::vec3(0, 0, 1));
	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 50,200 }), glm::vec3(1, 0, 0));
	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 50,150 }), glm::vec3(0, 0, 1));
	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 50,50 }), glm::vec3(1, 1, 0));
	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 150,50 }), glm::vec3(0, 0, 1));
	DrawLine(glm::ivec2({ 50,350 }), glm::ivec2({ 350,350 }), glm::vec3(0, 1, 1));
	DrawLine(glm::ivec2({ 50,50 }), glm::ivec2({ 350,50 }), glm::vec3(0, 1, 1));
	DrawLine(glm::ivec2({ 50,350 }), glm::ivec2({ 50,50 }), glm::vec3(0, 1, 1));
}

int Renderer::GetViewportWidth() const
{
	return viewport_width_;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height_;
}

void Renderer::drawAxis(const glm::mat4 & projectionMatrix, const glm::mat4 & viewMatrix)
{
	int half_width = viewport_width_ / 2;
	int half_height = viewport_height_ / 2;
	
	glm::vec3 xAxis(300.0, 0.0, 0.0);
	glm::vec3 yAxis(0.0,300.0, 0.0);
	glm::vec3 zAxis(0.0, 0.0, 300.0);
	glm::vec3 center(0.0, 0.0, 0.0);
	const glm::mat4x4 MMM = glm::mat4x4(
		{ 1,0,0,0 },
		{ 0,1,0,0 },
		{ 0,0,1,0 },
		{ half_width,half_height,0,1 }
	);
	glm::mat4 transformMat =projectionMatrix * viewMatrix ;

	//glm::vec4 centerT = transformMat * center;
	//centerT = centerT / center.w;
	//glm::vec4 xAxisT = transformMat * xAxis;
	//xAxisT = xAxisT / xAxisT.w;
	//glm::vec4 yAxisT = transformMat * yAxis;
	//yAxisT = yAxisT / yAxisT.w;
	//glm::vec4 zAxisT = transformMat * zAxis;
	//zAxisT = zAxisT / zAxisT.w;

	glm::vec3 xAxisT = trasformVec3(transformMat, xAxis);
	glm::vec3 yAxisT = trasformVec3(transformMat, yAxis);
	glm::vec3 zAxisT = trasformVec3(transformMat, zAxis);
	glm::vec3 centerT = trasformVec3(transformMat, center);


	DrawLine(glm::vec2(centerT.x, centerT.y), glm::vec2(xAxisT.x, xAxisT.y), glm::vec3(1, 0, 0));
	DrawLine(glm::vec2(centerT.x, centerT.y), glm::vec2(yAxisT.x, yAxisT.y), glm::vec3(0, 1, 0));
	DrawLine(glm::vec2(centerT.x, centerT.y), glm::vec2(zAxisT.x, zAxisT.y), glm::vec3(0, 0, 1));
}

void Renderer::SetViewport(int width, int height)
{
	viewport_width_ = width;
	viewport_height_ = height;
	CreateBuffers(viewport_width_, viewport_height_);
}