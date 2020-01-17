#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "MeshModel.h"
#include "LightSource.h"

enum Shading { flat, gouraud, phong };
enum Fog { noFog ,linear , exponential, exponentialSquared};

struct objFog {
	Fog fogType;
	float fog_maxdist;
	float fog_mindist;
	glm::vec3  fog_color;
	float density;
};

struct antiAlising {
	bool active;
	int k; 
};

class Scene {

public:
	Scene();

	void AddModel(const std::shared_ptr<MeshModel>& mesh_model);
	int GetModelCount() const;
	MeshModel& GetModel(int index) const;
	MeshModel& GetActiveModel() const;
	
	void AddCamera(const std::shared_ptr<Camera>& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index) const;
	Camera & GetActiveCamera() const;

	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void AddLight(const std::shared_ptr<LightSource>& light);
	int GetLightCount() const;
	LightSource & GetLight(int index) const;
	LightSource & GetActiveLight() const;

	void SetActiveLightIndex(int index);
	int GetActiveLightIndex() const;

	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;

	void changeFacesNormalsMode();
	void changeVerticesNormalsMode();

	bool getFacesNormalsStatus() const;
	bool getVerticesNormalsStatus() const;

	bool activeFacesNormals;
    bool activeVerticesNormals;

	void setBoudBox();

	bool getBoundBoxStatus() const;
	void setZoomForCam(float zoom, int index);
	bool activeBoundBox;

	void setShading(Shading _type);
	Shading getSahding() const;

	Fog getFogType() const;
	float getFogStart() const;
	float getFogEnd() const;
	float getFogDensity() const;
	glm::vec3 getFogColor() const;
	void setFogType(const Fog & _fogType);
	void setFogStart(const float _start);
	void setFogEnd(const float _end);
	void setFogDensity(const float _density);
	void setFogColor(const glm::vec3 & _color);
	objFog getFogObject() const;
	void AASwitch(const bool _mode);
	void setAAk(int _k);
	bool getAAMode() const;
	int getAAk() const;
	antiAlising getAAobj() const;

private:
	std::vector<std::shared_ptr<MeshModel>> mesh_models_;
	std::vector<std::shared_ptr<Camera>> cameras_;
	std::vector<std::shared_ptr<LightSource>> lights_sources_;

	int active_camera_index_;
	int active_model_index_;
	int active_lights_sources_index_;

	Shading shading_type;

	objFog fog;
	antiAlising AA;
};