#include "LightSource.h"

LightSource::LightSource(glm::vec3 _position, glm::vec3 _color, bool _pointSource, glm::vec3 _target) :
	position(_position),
	color(_color),
	pointSource(_pointSource),
	target(_target),
	intense({ glm::vec3(0.2f, 0.2f, 0.2f),glm::vec3(0.5f, 0.5f, 0.5f),glm::vec3(1.0f, 1.0f, 1.0f) }),
	scaleLight(glm::vec3(1.0f, 1.0f, 1.0f)),
	rotateLight(glm::vec3(0.0f, 0.0f, 0.0f))
{
}

LightSource::~LightSource()
{
}

void LightSource::setPosition(const glm::vec3 & _position)
{
	position = _position;
}

void LightSource::setScaling(const float _scale)
{
	setScaling(glm::vec3(_scale, _scale, _scale));
}


void LightSource::setScaling(const glm::vec3 & _scale)
{
	scaleLight = _scale;
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

void LightSource::setDiffuse(const glm::vec3 & _diffuse)
{
	intense.diffuse = _diffuse;
}

void LightSource::setSpecular(const glm::vec3 & _specular)
{
	intense.specular = _specular;
}

void LightSource::setAmbient(const glm::vec3 & _ambient)
{
	intense.ambient = _ambient;
}

glm::vec3 LightSource::getPosition() const
{
	return position;
}

glm::vec3 LightSource::getScaling() const
{
	return scaleLight;
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

glm::vec3 LightSource::getAmbient() const
{
	return intense.ambient;
}

glm::vec3 LightSource::getDiffuse() const
{
	return intense.diffuse;
}

glm::vec3 LightSource::getSpecular() const
{
	return intense.specular;
}

lightIntense LightSource::getLightIntenseStruct() const
{
	return intense;
}
