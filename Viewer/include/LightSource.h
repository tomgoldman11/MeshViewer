#pragma once
#include "vector"
#include <string>
#include <glm/glm.hpp>

struct lightIntense {
	glm::vec3 ambient; // <ambient_light_intensity (La)> 
	glm::vec3 diffuse; // <diffuse_light_intensity (Ld)> 
	glm::vec3 specular;  // <specular_light_intensity (Ls)> 
};

class LightSource
{
public:
	LightSource(glm::vec3 _position, glm::vec3 _color, bool _pointSource, glm::vec3 _target);
	~LightSource();

	//setters
	void setScaling(const float _scale);
	void setPosition(const glm::vec3 & _position);
	void setScaling(const glm::vec3 & _scale);
	void setColor(const glm::vec3 & _color);
	void setTarget(const glm::vec3 & _target);
	void chageType();
	void setDiffuse(const glm::vec3 & _diffuse);
	void setSpecular(const glm::vec3 & _specular);
	void setAmbient(const glm::vec3 & _specular);

	// getters
	glm::vec3 getPosition() const;
	glm::vec3 getScaling() const;
	glm::vec3 getColor() const;
	glm::vec3 getTarget() const;
	bool getType() const;
	glm::vec3 getAmbient() const;
	glm::vec3 getDiffuse() const;
	glm::vec3 getSpecular() const;
	lightIntense getLightIntenseStruct() const;

private:
	glm::vec3 position;
	glm::vec3 color;
	bool pointSource;
	glm::vec3 target;
	lightIntense intense;
	glm::vec3 scaleLight;
	glm::vec3 rotateLight;

};