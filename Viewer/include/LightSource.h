#pragma once
#include "vector"
#include <string>
#include <glm/glm.hpp>

class LightSource
{
public:
	LightSource(glm::vec3 _position, glm::vec3 _color, bool _pointSource, glm::vec3 _target);
	~LightSource();

	//setters
	void setPosition(const glm::vec3 & _position);
	void setColor(const glm::vec3 & _color);
	void setTarget(const glm::vec3 & _target);
	void chageType();

	// getters
	glm::vec3 getPosition() const;
	glm::vec3 getColor() const;
	glm::vec3 getTarget() const;
	bool getType() const;

private:
	glm::vec3 position;
	glm::vec3 color;
	bool pointSource;
	glm::vec3 target;
};