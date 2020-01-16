#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() :
	active_camera_index_(0),
	active_lights_sources_index_(0),
	active_model_index_(0),
	activeFacesNormals(false),
	activeVerticesNormals(true),
	activeBoundBox(false),
	shading_type(flat), //flat, gouraud, phong
	ambientLight(1.0,1.0,1.0,1.0),
	AA({ false,1 }),
	fog({ noFog, 1.00191271f , 1.0018f , glm::vec3(0.4, 0.4, 0.4) , 0.05f }) // type (noFog ,linear , exponential, exponentialSquered) , max, min, color //the default value match to our perspective view
{

}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
	mesh_models_.push_back(mesh_model);
}

int Scene::GetModelCount() const
{
	return mesh_models_.size();
}

MeshModel& Scene::GetModel(int index) const
{
	return *mesh_models_[index];
}

MeshModel& Scene::GetActiveModel() const
{
	return *mesh_models_[active_model_index_];
}

void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
	cameras_.push_back(camera);
}

int Scene::GetCameraCount() const
{
	return cameras_.size();
}

Camera& Scene::GetCamera(int index) const
{
	return *cameras_[index];
}

Camera& Scene::GetActiveCamera() const
{
	return *cameras_[active_camera_index_];
}

void Scene::SetActiveCameraIndex(int index)
{
	active_camera_index_ = index;
}

int Scene::GetActiveCameraIndex() const
{
	return active_camera_index_;
}

void Scene::AddLight(const std::shared_ptr<LightSource>& light)
{
	lights_sources_.push_back(light);
}

int Scene::GetLightCount() const
{
	return lights_sources_.size();
}

LightSource & Scene::GetLight(int index) const
{
	if (index < 0 || index >= lights_sources_.size()) {
		return *lights_sources_[0];
	}
	return *lights_sources_[index];
}

LightSource & Scene::GetActiveLight() const
{
	return *lights_sources_[active_lights_sources_index_];
}

void Scene::SetActiveLightIndex(int index)
{
	active_lights_sources_index_ = index;
}

int Scene::GetActiveLightIndex() const
{
	return active_lights_sources_index_;
}


void Scene::SetActiveModelIndex(int index)
{
	active_model_index_ = index;
}

int Scene::GetActiveModelIndex() const
{
	return active_model_index_;
}

void Scene::changeFacesNormalsMode()
{
	activeFacesNormals = !activeFacesNormals;
}

void Scene::changeVerticesNormalsMode()
{
	activeVerticesNormals = !activeVerticesNormals;
}

bool Scene::getFacesNormalsStatus() const
{
	return activeFacesNormals;
}

bool Scene::getVerticesNormalsStatus() const
{
	return activeVerticesNormals;
}

void Scene::setBoudBox()
{
	activeBoundBox = !activeBoundBox;
}

bool Scene::getBoundBoxStatus() const
{
	return activeBoundBox;
}

void Scene::setZoomForCam(float zoom, int index)
{
	cameras_[index]->setZoom(zoom);
}

void Scene::setShading(Shading _type)
{
	shading_type = _type;
}

Shading Scene::getSahding() const
{
	return shading_type;
}

Fog Scene::getFogType() const
{
	return fog.fogType;
}

float Scene::getFogStart() const
{
	return fog.fog_mindist;
}

float Scene::getFogEnd() const
{
	return fog.fog_maxdist;
}

float Scene::getFogDensity() const
{
	return fog.density;
}

glm::vec3 Scene::getFogColor() const
{
	return fog.fog_color;
}

void Scene::setFogType(const Fog & _fogType)
{
	fog.fogType = _fogType;
}

void Scene::setFogStart(const float _start)
{
	fog.fog_mindist = _start;
}

void Scene::setFogEnd(const float _end)
{
	fog.fog_maxdist = _end;
}

void Scene::setFogDensity(const float _density)
{
	fog.density = _density;
}

void Scene::setFogColor(const glm::vec3 & _color)
{
	fog.fog_color = _color;
}

objFog Scene::getFogObject() const
{
	return fog;
}

void Scene::AASwitch(const bool _mode)
{
	AA.active = _mode;
}

void Scene::setAAk(int _k)
{
	AA.k = _k;
}

antiAlising Scene::getAAobj() const
{
	return AA;
}

bool Scene::getAAMode() const
{
	return AA.active;
}

int Scene::getAAk() const
{
	return AA.k;
}
