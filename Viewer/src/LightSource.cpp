#include "LightSource.h"

LightSource::LightSource(glm::vec3 _position, glm::vec3 _color, bool _pointSource, glm::vec3 _target) :
	position(_position),
	color(_color),
	pointSource(_pointSource),
	target(_target),
	intense({0.9,0.9,0.9})
{
}

LightSource::~LightSource()
{
}

void LightSource::setPosition(const glm::vec3 & _position)
{
	position = _position;
}

void LightSource::setColor(const glm::vec3 & _color)
{
	color = _color;
}

void LightSource::setTarget(const glm::vec3 & _target)
{
	target = _target;
}

void LightSource::chageType()
{
	pointSource = !pointSource;
}

void LightSource::setDiffuse(const float _diffuse)
{
	intense.diffuse = _diffuse;
}

void LightSource::setSpecular(const float _specular)
{
	intense.specular = _specular;
}

void LightSource::setAmbient(const float _ambient)
{
	intense.ambient = _ambient;
}

glm::vec3 LightSource::getPosition() const
{
	return position;
}

glm::vec3 LightSource::getColor() const
{
	return color;
}

glm::vec3 LightSource::getTarget() const
{
	return target;
}

bool LightSource::getType() const
{
	return pointSource;
}

float LightSource::getAmbient() const
{
	return intense.ambient;
}

float LightSource::getDiffuse() const
{
	return intense.diffuse;
}

float LightSource::getSpecular() const
{
	return intense.specular;
}

lightIntense LightSource::getLightIntenseStruct() const
{
	return intense;
}
