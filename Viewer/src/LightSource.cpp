#include "LightSource.h"

LightSource::LightSource(glm::vec3 _position, glm::vec3 _color, bool _pointSource, glm::vec3 _target) :
	position(_position),
	color(_color),
	pointSource(_pointSource),
	target(_target)
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
