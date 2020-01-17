#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include "Renderer.h"
#include "InitShader.h"
#include "Utils.h"
#define NORMALS_LENGTHV 0.2f
#define NORMALS_LENGTHF 0.2f
#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width_(viewport_width),
	viewport_height_(viewport_height),
	shabeng(NULL),
	rendAA({ false,1 })
{
	InitOpenGLRendering();
	CreateBuffers(viewport_width, viewport_height);
	DrawLine(glm::vec3(0, 0,0), glm::vec3(1000, 1000,0), glm::vec3(0, 0, 0));
}

Renderer::~Renderer()
{
	delete[] color_buffer_;
}


void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;

	if (rendAA.active)
	{
		int pxCount = 1;
		std::map<std::pair<int, int>, zColor>::iterator neighborPixel;
		glm::vec3 finalColor(color);
		if (i % 2 == 0)
		{
			if (j % 2 == 0) neighborPixel = Mapix.find(std::make_pair(i + 1, j - 1));
			else neighborPixel = Mapix.find(std::make_pair(i + 1, j + 1));
		}
		else
		{
			if (j % 2 == 0) neighborPixel = Mapix.find(std::make_pair(i - 1, j + 1));
			else neighborPixel = Mapix.find(std::make_pair(i - 1, j - 1));
		}
		//if (shabeng.empty())
		//{
		//	color_buffer_[INDEX(viewport_width_, i, j, 0)] = 0;
		//	color_buffer_[INDEX(viewport_width_, i, j, 1)] = 0;
		//	color_buffer_[INDEX(viewport_width_, i, j, 2)] = 0;
		//	return;
		//}
		//for (int r = i- rendAA.k; r <= i+ rendAA.k; r+=2)
		//{
		//	if (r < 0) continue; if (r >= viewport_width_) continue;
		//	for (int c = j- rendAA.k; c <= j+ rendAA.k; c+=2)
		//	{
		//		if (r == i && c == j) continue;
		//		if (rand() % 5 != 1) continue;
		//		if (c < 0) continue; if (c >= viewport_height_) continue;
		//		std::map<std::pair<int, int>, zColor>::iterator neighborPixel = Mapix.find(std::make_pair(r, c));
				if (neighborPixel != Mapix.end())
				{
					finalColor += neighborPixel->second.color;
					++pxCount;
				}
				else
				{
					finalColor += glm::vec3(0.8, 0.8, 0.8);
					++pxCount;
				}
		//		
		//	}
		//}
		finalColor /= 2;
		color_buffer_[INDEX(viewport_width_, i, j, 0)] = finalColor.x;
		color_buffer_[INDEX(viewport_width_, i, j, 1)] = finalColor.y;
		color_buffer_[INDEX(viewport_width_, i, j, 2)] = finalColor.z;

	}
	else
	{
		color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
		color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
		color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
	}
}

glm::vec3 Renderer::GetPixel(int i, int j)
{
	if (i < 0) return glm::vec3(0,0,0); if (i >= viewport_width_) return glm::vec3(0,0,0);
	if (j < 0) return glm::vec3(0,0,0); if (j >= viewport_height_) return glm::vec3(0, 0, 0);

	glm::vec3 retVec (color_buffer_[INDEX(viewport_width_, i, j, 0)], color_buffer_[INDEX(viewport_width_, i, j, 1)], color_buffer_[INDEX(viewport_width_, i, j, 2)]);
	return retVec;
}

void Renderer::SetViewportHeight(const int _viewport_height)
{
	viewport_height_ = _viewport_height;
}

void Renderer::SetViewportWidth(const int _viewport_width)
{
	viewport_width_ = _viewport_width;
}

void Renderer::DrawLine(const glm::ivec3& p1, const glm::ivec3& p2, const glm::vec3& color)
{
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	float x1 = float(p1.x);
	float y1 = float(p1.y);
	float x2 = float(p2.x);
	float y2 = float(p2.y);
	dx = int(x2 - x1);
	dy = int(y2 - y1);
	dx1 = int(fabs(dx));
	dy1 = int(fabs(dy));
	px = 2 * dy1 - dx1;
	py = 2 * dx1 - dy1;
	if (dy1 <= dx1) // delta y-s is smaller or equal to delta x-s
	{
		if (dx >= 0)// x2 >= x1
		{
			x = int(x1);
			y = int(y1);
			xe = int(x2);
		}
		else // x2 < x1
		{
			x = int(x2);
			y = int(y2);
			xe = int(x1);
		}
		PutPixel(x, y, color);
		for (i = 0; x < xe; i++)
		{
			//getZOnline

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
	else // delta y-s is larger to delta x-s 
	{
		if (dy >= 0)// x2 >= x1
		{
			x = int(x1);
			y = int(y1);
			ye = int(y2);
		}
		else // x2 < x1
		{
			x = int(x2);
			y = int(y2);
			ye = int(y1);
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
void Renderer::ALTER_DrawLine(const glm::ivec3& p1, const glm::ivec3& p2, const glm::vec3& color)
{
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	float x1 = float(p1.x);
	float y1 = float(p1.y);
	float x2 = float(p2.x);
	float y2 = float(p2.y);
	float z1 = float(p1.z);
	float z2 = float(p2.z);
	float z = 0.0f;
	dx = int(x2 - x1);
	dy = int(y2 - y1);
	dx1 = int(fabs(dx));
	dy1 = int(fabs(dy));
	px = 2 * dy1 - dx1;
	py = 2 * dx1 - dy1;
	
	if (dy1 <= dx1) // delta y-s is smaller or equal to delta x-s
	{
		if (dx >= 0)// x2 >= x1
		{
			x = int(x1);
			y = int(y1);
			xe = int(x2);
		}
		else // x2 < x1
		{
			x = int(x2);
			y = int(y2);
			xe = int(x1);
		}
		z = getZOnLine(x, y, x1, y1, z1, x2, y2, z2);
		addToMapix(x, y, z, color);
		for (i = 0; x < xe; i++)
		{
			//getZOnline

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
			z = getZOnLine(x, y, x1, y1, z1, x2, y2, z2);
			addToMapix(x, y, z, color);
		}
	}
	else // delta y-s is larger to delta x-s 
	{
		if (dy >= 0)// x2 >= x1
		{
			x = int(x1);
			y = int(y1);
			ye = int(y2);
		}
		else // x2 < x1
		{
			x = int(x2);
			y = int(y2);
			ye = int(y1);
		}
		z = getZOnLine(x, y, x1, y1, z1, x2, y2, z2);
		addToMapix(x, y, z, color);
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
			z = getZOnLine(x, y, x1, y1, z1, x2, y2, z2);
			addToMapix(x, y, z, color);
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

glm::vec4 Renderer::trasformVec3(const glm::mat4& transformationMatrix, glm::vec3 vector)
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

glm::vec3 Renderer::drawFacesNormals(const glm::vec3& vec1, const glm::vec3& vec2, const glm::vec3& vec3, const glm::mat4x4& transformationMatrix, const Face& currFace, const bool print_normals)
{
	glm::vec3 newPoint = (vec1 + vec2 + vec3) / 3.0f;

	glm::vec3 newPoint_T = trasformVec3(transformationMatrix, newPoint);

	glm::vec3 _normalPoint = currFace.getNormal();
	glm::vec4 normalPoint = glm::vec4(_normalPoint.x, _normalPoint.y, _normalPoint.z, 0.0f);

	glm::vec3 normal = NORMALS_LENGTHF * _normalPoint + newPoint;
	normal = trasformVec3(transformationMatrix, normal);
	//DrawLine(newPoint_T, normal, glm::vec3(0, 1, 0));
	if (print_normals) {
		ALTER_DrawLine(newPoint_T, normal, glm::vec3(0, 1, 0));
	}
	return glm::normalize(normal - newPoint_T);
}

void Renderer::drawVerticesNormals(const MeshModel & mesh, const std::vector<glm::vec3> & vertices, const glm::mat4x4& transformationMatrix, const bool print_normals)
{
	for (int i = 0; i < mesh.getVerticesCount(); ++i) {
		glm::vec3 vVec = vertices[i];
		glm::vec3 sumNormals3 = NORMALS_LENGTHV * mesh.getVertexNormal(i) + vVec;
		vVec = trasformVec3(transformationMatrix, vVec);
		sumNormals3 = trasformVec3(transformationMatrix, sumNormals3);

		if (print_normals) {
			ALTER_DrawLine(vVec, sumNormals3, glm::vec3(0, 1, 0));
		}
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
	ALTER_DrawLine(XnYZ, XYZ, glm::vec3(0, 0, 1));
	ALTER_DrawLine(XnYZ, XnYnZ, glm::vec3(0, 0, 1));
	ALTER_DrawLine(XnYZ, nXnYZ, glm::vec3(0, 0, 1));

	ALTER_DrawLine(nXYnZ, nXYZ, glm::vec3(0, 0, 1));
	ALTER_DrawLine(nXYnZ, XYnZ, glm::vec3(0, 0, 1));
	ALTER_DrawLine(nXYnZ, nXnYnZ, glm::vec3(0, 0, 1));

	ALTER_DrawLine(XnYnZ, nXnYnZ, glm::vec3(0, 0, 1));
	ALTER_DrawLine(XnYnZ, XYnZ, glm::vec3(0, 0, 1));

	ALTER_DrawLine(nXYZ, nXnYZ, glm::vec3(0, 0, 1));
	ALTER_DrawLine(nXYZ, XYZ, glm::vec3(0, 0, 1));

	ALTER_DrawLine(nXnYnZ, nXnYZ, glm::vec3(0, 0, 1));
	ALTER_DrawLine(XYnZ, XYZ, glm::vec3(0, 0, 1));

}

// flood fill algorithm 
void Renderer::flood(int x, int y, const glm::vec3& new_col, const glm::vec3& sidesColor, Face & currFace)
{
	int centerX = x, centeryY = y;
	std::queue<std::pair<int, int>> pixels;
	//std::vector<std::pair<int, int>> visited;

	pixels.push(std::make_pair(x, y));
	//visited.push_back(std::make_pair(x, y));

	// check current pixel is old_color or not 
	while (!pixels.empty()) {

		// put new pixel with new color
		x = pixels.front().first;
		y = pixels.front().second;

		pixels.pop();
		if (GetPixel(x,y) != sidesColor && GetPixel(x, y) != new_col) {

			//PutPixel(x, y, new_col);


			//if (std::find(visited.begin(), visited.end(), std::make_pair(x, y + 1)) == visited.end()) {
				pixels.push(std::make_pair(x, y + 1));
			//	visited.push_back(std::make_pair(x, y + 1));
			//}
			//if (std::find(visited.begin(), visited.end(), std::make_pair(x -1, y)) == visited.end()) {
				pixels.push(std::make_pair(x - 1, y));
			//	visited.push_back(std::make_pair(x - 1, y));
			//}
			//if (std::find(visited.begin(), visited.end(), std::make_pair(x + 1, y)) == visited.end()) {
				pixels.push(std::make_pair(x + 1, y));
			//	visited.push_back(std::make_pair(x + 1, y));
			//}
			//if (std::find(visited.begin(), visited.end(), std::make_pair(x, y - 1)) == visited.end()) {
				pixels.push(std::make_pair(x, y - 1));
			//	visited.push_back(std::make_pair(x, y - 1));
			//}


		}
	}

	//if (GetPixel(x, y) != sidesColor && GetPixel(x, y) != new_col) {

		//PutPixel(x, y, new_col);

		//// recursive call for top pixel fill
		//if (GetPixel(x, y + 1) != sidesColor && GetPixel(x, y + 1) != new_col) {
		//	flood(x, y + 1, new_col, sidesColor);
		//}

		//// recursive call for left pixel fill 
		//if (GetPixel(x-1, y) != sidesColor && GetPixel(x - 1, y) != new_col) {
		//	flood(x - 1, y, new_col, sidesColor);
		//}
		//// recursive call for right pixel fill 
		//if (GetPixel(x + 1, y) != sidesColor && GetPixel(x + 1, y) != new_col) {
		//	flood(x + 1, y, new_col, sidesColor);
		//}
		//// recursive call for bottom pixel fill 
		//if (GetPixel(x, y - 1) != sidesColor && GetPixel(x, y - 1) != new_col) {
		//	flood(x, y - 1, new_col, sidesColor);
		//}
	//}
}

void Renderer::drawFaceTriangle(const glm::vec3 & vec1, const glm::vec3 & vec2, const glm::vec3 & vec3, const glm::mat4x4 & transformationMatrix, const Face & currFace, const glm::vec3 &color)
{
	glm::vec3 vec1T = trasformVec3(transformationMatrix, vec1);
	glm::vec3 vec2T = trasformVec3(transformationMatrix, vec2);
	glm::vec3 vec3T = trasformVec3(transformationMatrix, vec3);
	
	// draw the triangle
	if (currFace.getVerticesCount() == 3) {
		ALTER_DrawLine(vec1T, vec2T, color);
		ALTER_DrawLine(vec2T, vec3T, color);
		ALTER_DrawLine(vec3T, vec1T, color);
	}

}

void Renderer::addToMapix(int x, int y, float z, const glm::vec3 & color, bool addColorFlag)
{
	if (x < 0) return; if (x >= viewport_width_) return;
	if (y < 0) return; if (y >= viewport_height_) return;

	float _distance = std::sqrt(pow((x - eyePoint.x), 2) + pow((y - eyePoint.y), 2) + pow((z - eyePoint.z), 2));

	std::map<std::pair<int, int>, zColor>::iterator pixel = Mapix.find(std::make_pair(x, y));
	int currentPixel = x * viewport_width_ + y ;
	int colorIndex = 3 * currentPixel;

	if (pixel == Mapix.end()) { //if it doesnt exist...
		Mapix[std::make_pair(x, y)] = zColor({ _distance, color });
		return;
	}
	
	if (_distance > pixel->second.z) {
		Mapix[std::make_pair(x, y)] = zColor({ _distance, color });
		return;
	}

	if (_distance == pixel->second.z) {
		Mapix[std::make_pair(x, y)].color += color;
		return;
	}
	//if (_distance > zBuffer[currentPixel]) {
	//	shabeng[colorIndex + 0] = color.x;
	//	shabeng[colorIndex + 1] = color.y;
	//	shabeng[colorIndex + 2] = color.z;
	//	zBuffer[currentPixel] = _distance;
	//	return;
	//}

	//if (_distance == zBuffer[currentPixel]) {
	//	shabeng[colorIndex + 0] += color.x;
	//	shabeng[colorIndex + 1] += color.y;
	//	shabeng[colorIndex + 2] += color.z;
	//	return;
	//}

}

void Renderer::addColor(const Shading & shadingType, const glm::mat4x4 & helper, const LightSource & currentLight, const glm::vec3 &faceNormal, const triangleVecs & origTriangle, const material & meshMaterialAttr, const triangleVecsNormals & origVerticesNormals, const glm::mat4x4 & transformationMatrix, const bool & printVerticesNormals, const glm::vec3 & faceCenter, const objFog & fog, const texture & modelTexture) // find another implementation
{
	glm::vec3 Tp1 = trasformVec3(transformationMatrix, origTriangle.vec1P);
	glm::vec3 Tp2 = trasformVec3(transformationMatrix, origTriangle.vec2P);
	glm::vec3 Tp3 = trasformVec3(transformationMatrix, origTriangle.vec3P);
	float a = 0.0f, b = 0.0f, c = 0.0f, zPoint, fog_factor;
	bool changed = false;
	glm::vec3 norm1(0), norm2(0), norm3(0), result(0), color(0);
	glm::vec3 faceCenterT = trasformVec3(transformationMatrix, faceCenter);

	int minX = int(std::min(std::min(Tp1.x, Tp2.x), Tp3.x));
	int minY = int(std::min(std::min(Tp1.y, Tp2.y), Tp3.y));
	int maxX = int(std::max(std::max(Tp1.x, Tp2.x), Tp3.x));
	int maxY = int(std::max(std::max(Tp1.y, Tp2.y), Tp3.y));

	norm1 = drawVertixNormal(origTriangle.vec1P, origVerticesNormals.vec1N, transformationMatrix, printVerticesNormals);
	norm2 = drawVertixNormal(origTriangle.vec2P, origVerticesNormals.vec2N, transformationMatrix, printVerticesNormals);
	norm3 = drawVertixNormal(origTriangle.vec3P, origVerticesNormals.vec3N, transformationMatrix, printVerticesNormals);

	for (int x = minX; x <= maxX; ++x) {
		for (int y = maxY; y >= minY; --y) {
			if (Utils::PointInTriangle(glm::vec2(x, y), Tp1, Tp2, Tp3, &zPoint)) {
				switch (shadingType)
				{
				case flat:
					color = getFaceChanger(helper, currentLight, faceNormal, origTriangle, meshMaterialAttr, faceCenterT, modelTexture);
					break;
				case gouraud:
					Utils::getLinearInterpolationOfPoints(faceCenterT.x, faceCenterT.y, Tp1, Tp2, Tp3, &a, &b, &c, &changed);
					if (changed == false) continue;
					glm::vec3 color1 = getFaceChanger(helper, currentLight, norm1, origTriangle, meshMaterialAttr, glm::vec3(x, y, zPoint), modelTexture);
					glm::vec3 color2 = getFaceChanger(helper, currentLight, norm2, origTriangle, meshMaterialAttr, glm::vec3(x, y, zPoint), modelTexture);
					glm::vec3 color3 = getFaceChanger(helper, currentLight, norm3, origTriangle, meshMaterialAttr, glm::vec3(x, y, zPoint), modelTexture);
					color = a * color1 + b * color2 + c * color3;
					break;
				case phong:
					Utils::getLinearInterpolationOfPoints(x, y, Tp1, Tp2, Tp3, &a, &b, &c, &changed);
					if (changed == false) continue;
					glm::vec3 superNormal = a * norm1 + b * norm2 + c * norm3;

					color = getFaceChanger(helper, currentLight, superNormal, origTriangle, meshMaterialAttr, glm::vec3(x, y, zPoint), modelTexture);
				}

				switch (fog.fogType)
				{
				case linear:
					fog_factor = (fog.fog_maxdist - zPoint) / (fog.fog_maxdist - fog.fog_mindist);
					if (fog_factor < 0.0) fog_factor = 0;
					else if (fog_factor > 1.0) fog_factor = 1;
					
					color = (1 - fog_factor) * fog.fog_color + fog_factor * color;

					break;
				case exponential:
					fog_factor = 1 / (exp(fog.density*zPoint));

					color = (1 - fog_factor) * fog.fog_color + fog_factor * color;

					break;
				case exponentialSquared:
					fog_factor = 1 / (exp(pow(fog.density*zPoint,2)));

					color = (1 - fog_factor) * fog.fog_color + fog_factor * color;

					break;
				default:
					// none fog
					break;
				}

				addToMapix(x, y, zPoint, color);
				if (zPoint < minZ) {
					minZ = zPoint;
				}
				if (zPoint > maxZ) {
					maxZ = zPoint;
				}
			}
		}
	}

}

glm::vec3 Renderer::getFaceChanger(const glm::mat4x4 & globalTransformationMatrix, const LightSource & light, const glm::vec3 & normalTEST, const triangleVecs & currTriangle, const material & _materialAttr, const glm::vec3 & point,const texture & modelTexture)
{
	float dotProd = 0.0f;
	glm::vec3 lightDir;
	glm::vec3 lightPos(globalTransformationMatrix * glm::vec4(light.getPosition(), 0.0f));
	glm::vec3 ambient(0);
	glm::vec3 newAmbient(0);
	//ambient
	switch (modelTexture) {
	case uniform:
		ambient = _materialAttr.ambient * light.getAmbient();
		break;
	case non_uniform:
		newAmbient= glm::vec3(cos(currTriangle.vec1P.x*1000), cos(currTriangle.vec2P.y*1000), cos(currTriangle.vec3P.z*1000));
		ambient = newAmbient * light.getAmbient();
		break;
	case random_uniform:
		newAmbient = glm::vec3(cos(point.x), cos(point.y), cos(point.z));
		ambient = newAmbient * light.getAmbient(); 
		break;
	}


	//diffuse
	if (light.getType()) {
		lightDir = glm::normalize(lightPos - point);
	}
	else 
	{
		lightDir = glm::normalize(lightPos - light.getTarget());
	}

	dotProd = glm::dot(lightDir, normalTEST);

	float diff1 = (dotProd > 0.0f) ? dotProd : 0.0f;
	glm::vec3 diffuse = _materialAttr.diffuse * diff1 * light.getDiffuse();


	// specular
	glm::vec3 viewDir = glm::normalize(point - glm::vec3(eyePoint));
	glm::vec3 reflectDir = glm::normalize(glm::reflect(-lightDir, normalTEST));
	dotProd = dot(reflectDir, viewDir);
	float diff = (dotProd > 0.0f) ? dotProd : 0.0f;
	float spec = pow(abs(diff), _materialAttr.shininess);
	glm::vec3 specular = _materialAttr.specular * spec * light.getSpecular();

	glm::vec3 result = (ambient + diffuse + specular);

	return result;
}

glm::vec3 Renderer::generateNewColor(const glm::mat4x4 & globalTransformationMatrix, const std::vector<std::shared_ptr<LightSource>>& lights_sources, const glm::mat4x4 & transformationMatrix, const glm::vec3 & normalTEST, const glm::vec3 & eye, const triangleVecs & currTriangle, const material & _materialAttr)
{

	return glm::vec3();
}

glm::vec3 Renderer::drawVertixNormal(const glm::vec3 & vertex, const glm::vec3 & normal, const glm::mat4x4 & transformationMatrix, const bool print_normals)
{
	glm::vec3 sumNormals3 = NORMALS_LENGTHV * normal + vertex;
	glm::vec3 vVec = trasformVec3(transformationMatrix, vertex);
	sumNormals3 = trasformVec3(transformationMatrix, sumNormals3);

	if (print_normals) {
		ALTER_DrawLine(vVec, sumNormals3, glm::vec3(0, 1, 0));
	}

	return glm::normalize(sumNormals3 - vVec);
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
	rendAA = scene.getAAobj();
	// TODO: Replace this code with real scene rendering code
	Mapix.clear();
	//shabeng = std::vector<float>(viewport_width_ * viewport_height_ * 3,0.8f);
	//zBuffer = new float[viewport_width_ * viewport_height_ ];
	//std::fill(zBuffer, zBuffer + viewport_width_ * viewport_height_, -10000);
	farestKnownPoint = 0;
	int half_width = viewport_width_ / 2;
	int half_height = viewport_height_ / 2;
	std::map<int, std::vector<int>> verticesNormals;
	glm::mat4 transformationMatrix;
	const auto& activeCamera = scene.GetActiveCamera(); // getting the active camera in the current scene
	const auto& activeLight = scene.GetActiveLight();
	const glm::vec3 lightColor = activeLight.getColor();
	const glm::mat4x4 viewMatrix = glm::inverse(activeCamera.getViewTransformation());
    const glm::mat4x4 projectionMatrix = activeCamera.getProjectionTransformation();
	const glm::mat4x4 MMM = glm::mat4x4(
		{1,0,0,0},
		{ 0,1,0,0 },
		{ 0,0,1,0 },
		{ half_width,half_height,0,1 }
	);
	drawAxis(projectionMatrix, viewMatrix);
	const objFog sceneFog = scene.getFogObject();
	minZ = 1000, maxZ = 0;

	eyePoint = MMM * glm::vec4(activeCamera.getEye(), 1.0f);

	for (int i = 0; i < scene.GetModelCount(); i++) { // looping over the models
		MeshModel mesh = scene.GetModel(i);

		// init face normals
		mesh.setFaceNormals();

		//get the vertices
		std::vector<glm::vec3> vertices = mesh.getVertices();

		//get the M matrix (world frame) related to the mesh model
		const glm::mat4x4 modelMatrix = mesh.getLocalTransformation();
		const glm::mat4x4 worldlMatrix = mesh.getWorldTransformation();

		//set a 4X4 transform matrix for the faces T = P*V*M
		transformationMatrix =  projectionMatrix* viewMatrix *worldlMatrix * modelMatrix;

		material meshMaterialAttr = mesh.getObjMaterialStruct();

		glm::mat4x4 helper = MMM * projectionMatrix* viewMatrix;
		Shading shadingType = scene.getSahding();
		texture modelTexture = mesh.getTextureType();

		for (int j = 0; j < mesh.GetFacesCount(); j++) {
			Face currFace = mesh.GetFace(j);
			int v1 = currFace.GetVertexIndex(0) - 1;
			glm::vec3 vec1 = vertices[v1];
			int v2 = currFace.GetVertexIndex(1) - 1;
			glm::vec3 vec2 = vertices[v2];
			int v3 = currFace.GetVertexIndex(2) - 1;
			glm::vec3 vec3 = vertices[v3];

			triangleVecs currTriangle({ vec1,vec2,vec3 });
			triangleVecsNormals currVerticesNormals({ mesh.getVertexNormal(v1),mesh.getVertexNormal(v2),mesh.getVertexNormal(v3)});

			//// draw triagles
			//drawFaceTriangle(vec1, vec2, vec3, transformationMatrix, currFace, mesh.getSidesColor());

			//draw faces normals
			glm::vec3 normalTEST = drawFacesNormals(vec1, vec2, vec3, transformationMatrix, currFace, scene.getFacesNormalsStatus());
			glm::vec3 faceCenter = (currTriangle.vec1P + currTriangle.vec2P + currTriangle.vec3P) / 3.0f;

			//light
			glm::vec3 result(0);
			for (int indLight = 0; indLight < scene.GetLightCount(); ++indLight) { 
				addColor(shadingType, helper, scene.GetLight(indLight), normalTEST, currTriangle, meshMaterialAttr, currVerticesNormals, transformationMatrix, scene.getVerticesNormalsStatus(), faceCenter, sceneFog, modelTexture);
			}


		}
		//draw vertices normals
		if (scene.getVerticesNormalsStatus() && shadingType==flat)
		{
			drawVerticesNormals(mesh, vertices, transformationMatrix, scene.getVerticesNormalsStatus());
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
		std::shared_ptr<MeshModel> cameraModel = Utils::LoadMeshModel("D:\\Repositories\\mesh-viewer-tom-tal\\Data\\camera.obj");
		//std::shared_ptr<MeshModel> cameraModel = Utils::LoadMeshModel("D:\\graphics proj\\new\\mesh-viewer-tom-tal\\Data\\camera.obj");
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
			drawFaceTriangle(vec1, vec2, vec3, transformationMatrix, currFace, glm::vec3(0,0,0));
		}
	}

	for (int i = 0; i < scene.GetLightCount(); ++i)
	{
		LightSource&  lightObj = scene.GetLight(i);
		std::shared_ptr<MeshModel> lightModel = Utils::LoadMeshModel("D:\\Repositories\\mesh-viewer-tom-tal\\Data\\demo.obj");
		//std::shared_ptr<MeshModel> lightModel = Utils::LoadMeshModel("D:\\graphics proj\\new\\mesh-viewer-tom-tal\\Data\\demo.obj");

		//get the vertices
		std::vector<glm::vec3> vertices = lightModel->getVertices();
		lightModel->setTranslate_local(lightObj.getPosition());
		lightModel->setScale_local(lightObj.getScaling());


		//get the M matrix (world frame) related to the mesh model
		const glm::mat4x4 modelMatrix = lightModel->getLocalTransformation();
		const glm::mat4x4 worldlMatrix = lightModel->getWorldTransformation();

		//set a 4X4 transform matrix for the faces T = P*V*M
		transformationMatrix = projectionMatrix * viewMatrix * worldlMatrix * modelMatrix;

		for (int j = 0; j < lightModel->GetFacesCount(); j++) {
			Face currFace = lightModel->GetFace(j);
			int v1 = currFace.GetVertexIndex(0) - 1;
			glm::vec3 vec1 = vertices[v1];
			int v2 = currFace.GetVertexIndex(1) - 1;
			glm::vec3 vec2 = vertices[v2];
			int v3 = currFace.GetVertexIndex(2) - 1;
			glm::vec3 vec3 = vertices[v3];
			drawFaceTriangle(vec1, vec2, vec3, transformationMatrix, currFace, glm::vec3(0, 0, 0));
		}
	}


	//draw pixels
	for (std::map<std::pair<int, int>, zColor>::iterator itr = Mapix.begin(); itr != Mapix.end(); ++itr) 
	{
		PutPixel(itr->first.first, itr->first.second, itr->second.color);

		farestKnownPoint = (itr->second.z > farestKnownPoint) ? itr->second.z : farestKnownPoint;
	}

	//for (int r = 0; r < viewport_height_; ++r)
	//{
	//	for (int c = 0; c < viewport_width_; ++c) {
	//		int location = (r * viewport_width_ + c) * 3;
	//		glm::vec3 pixColor(shabeng[location + 0], shabeng[location + 1], shabeng[location + 2]);
	//		PutPixel(r, c, pixColor);
	//	}
	//}
	//shabeng.clear();
	//delete[] zBuffer;

}

//void Renderer::DrawAsterisk() // testing function for DrawLine
//{
//	DrawLine(glm::ivec2({ 350,350 }), glm::ivec2({ 350,50 }), glm::vec3(0, 1, 1));
//	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 200,50 }), glm::vec3(1, 0, 0));
//	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 250,50 }), glm::vec3(0, 0, 1));
//	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 350,50 }), glm::vec3(1, 1, 0));
//	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 350,150 }), glm::vec3(0, 0, 1));
//	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 350,200 }), glm::vec3(1, 0, 0));
//	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 350,250 }), glm::vec3(0, 0, 1));
//	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 350,350 }), glm::vec3(1, 1, 0));
//	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 250,350 }), glm::vec3(0, 0, 1));
//	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 200,350 }), glm::vec3(1, 0, 0));
//	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 150,350 }), glm::vec3(0, 0, 1));
//	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 50,350 }), glm::vec3(1, 1, 0));
//	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 50,250 }), glm::vec3(0, 0, 1));
//	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 50,200 }), glm::vec3(1, 0, 0));
//	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 50,150 }), glm::vec3(0, 0, 1));
//	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 50,50 }), glm::vec3(1, 1, 0));
//	DrawLine(glm::ivec2({ 200,200 }), glm::ivec2({ 150,50 }), glm::vec3(0, 0, 1));
//	DrawLine(glm::ivec2({ 50,350 }), glm::ivec2({ 350,350 }), glm::vec3(0, 1, 1));
//	DrawLine(glm::ivec2({ 50,50 }), glm::ivec2({ 350,50 }), glm::vec3(0, 1, 1));
//	DrawLine(glm::ivec2({ 50,350 }), glm::ivec2({ 50,50 }), glm::vec3(0, 1, 1));
//}

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
	glm::mat4 transformMat =  projectionMatrix * viewMatrix;

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


	DrawLine(centerT, xAxisT, glm::vec3(1, 0, 0));
	DrawLine(centerT, yAxisT, glm::vec3(0, 1, 0));
	DrawLine(centerT, zAxisT, glm::vec3(0, 0, 1));
}

void Renderer::SetViewport(int width, int height)
{
	viewport_width_ = width;
	viewport_height_ = height;
	CreateBuffers(viewport_width_, viewport_height_);
}

float Renderer::getZOnLine(int x, int y, int x1, int y1, float z1, int x2, int y2, float z2)
{
	float divBeta = float((y2 * x1 - y1 * x2));
	if (x1 == 0.0f || divBeta == 0.0f) return -100000;
	float beta = float((y * x1 - x * y1) / divBeta);
	float alpha = float((x - x2 * beta) / x1);
	float z = float(alpha * z1 + beta * z2);
	return z;
}

