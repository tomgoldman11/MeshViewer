#pragma once
#include "vector"
#include <string>
#include <glm/glm.hpp>

struct lightIntense {
	float ambient; // <ambient_light_intensity (La)> 
	float diffuse; // <diffuse_light_intensity (Ld)> 
	float specular;  // <specular_light_intensity (Ls)> 
};

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
	void setDiffuse(const float _diffuse);
	void setSpecular(const float _specular);
	void setAmbient(const float _specular);

	// getters
	glm::vec3 getPosition() const;
	glm::vec3 getColor() const;
	glm::vec3 getTarget() const;
	bool getType() const;
	float getAmbient() const;
	float getDiffuse() const;
	float getSpecular() const;
	lightIntense getLightIntenseStruct() const;

private:
	glm::vec3 position;
	glm::vec3 color;
	bool pointSource;
	glm::vec3 target;
	lightIntense intense;
};