#pragma once
#include <glm/glm.hpp>

struct Point
{
	double X;
	double Y;
	glm::vec3 COLOR;

	Point() : X(0), Y(0) {}
	Point(double x, double y) :X(x), Y(y) {}
	//Point(double x, double y, glm::vec3 color) :X(x), Y(y), COLOR(color) {}
};