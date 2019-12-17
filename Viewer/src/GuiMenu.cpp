#pragma once
#include "GuiMenu.h"
#include <stdio.h>
#include <stdlib.h>
#include <nfd.h>
#include "Scene.h"
#include "Utils.h"
#include "MeshModel.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Renderer.h"
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
/**
 * Fields
 */
bool show_demo_window = false;
bool show_scale_window = false;
bool show_rotate_window = false;
bool show_translate_window = false;
bool bounding_box = false;
bool normals_per_face = false;
bool normals_per_vertex = false;
bool ortho = false;
static int pers = 0; // 0 for ortho , 1 for perspective
static int WorldLocal = 0; // 0 for world , 1 for local

glm::vec4 clear_color1 = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

// model tranform
 float ScaleX = 1.0f;
 float ScaleY = 1.0f;
 float ScaleZ = 1.0f;
 float ScaleU = 1.0f;
 float RotateX = 0.0f;
 float RotateY = 0.0f;
 float RotateZ = 0.0f;
 float TranslateX = 0.0f;
 float TranslateY = 0.0f;
 float TranslateZ = 0.0f;

// camera transform
static float camX = 0.0f; // eye parameters
static float camY = 0.0f;
static float disZ = 1.0f;
static float atX = 0.0f;
static float atY = 0.0f;
static float atZ = 0.0f;
static float fovy = 50.0f; 
static float zoom = 1.0f;

/**
 * Function implementation
 */

const glm::vec4& GetClearColor()
{
	return clear_color1;
}

char* const stringToCharSeq(const std::string& str)
{
	int len = (int)str.length() + 1;
	char** seq = new char*[1];
	seq[0] = new char[len];
	for (int i = 0; i < len; ++i)
	{
		seq[0][i] = str[i];
	}
	return seq[0];
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	ImGui::SetNextWindowPos(ImVec2(0, 30), ImGuiCond_Once);
	/*
	 * MeshViewer menu
	 *
	 */
	ImGui::Begin("MeshViewer Menu");

	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Add"))
		{
			if (ImGui::MenuItem("add camera", "CTRL+A"))
			{
				scene.AddCamera(Utils::LoadCamera(glm::vec3{ 1,0,1 }, glm::vec3{ 0,0,0 }, glm::vec3{ 0,1,0 }));
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disable item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Show Demo Menu")) { show_demo_window = true; }
			ImGui::EndMenu();
		}
		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	// Controls
	ImGui::Text("Background Color");
	ImGui::SameLine();
	ImGui::ColorEdit3("Background", (float*)&clear_color1);

	ImGui::End(); // end meshviewer menu

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::Begin("Model Control");                          // Create a window called "Model Contorl" and append into it.
										
	if (scene.GetModelCount() == 0)
	{
		ImGui::Text("No models loaded yet");
		ImGui::End();
		return;
	}
	// multiple models
	static int currmod = 0;
	MeshModel* activeModel=&(scene.GetActiveModel());// getting the active model
	if (ImGui::InputInt("ActiveModel", &currmod))
	{
		if (currmod < 0) currmod = 0;
		if (currmod >= scene.GetModelCount()) currmod = scene.GetModelCount() - 1;
		scene.SetActiveModelIndex(currmod);
		activeModel = &(scene.GetActiveModel());
		if (WorldLocal == 0)
		{
			ScaleX = activeModel->getScaleVector_world().x;
			ScaleY = activeModel->getScaleVector_world().y;
			ScaleZ = activeModel->getScaleVector_world().z;
			ScaleU = activeModel->getScaleVector_world().x;
			RotateX = activeModel->getRotateVector_world().x;
			RotateY = activeModel->getRotateVector_world().y;
			RotateZ = activeModel->getRotateVector_world().z;
			TranslateX = activeModel->getTranslateVector_world().x;
			TranslateY = activeModel->getTranslateVector_world().y;
			TranslateZ = activeModel->getTranslateVector_world().z;
		}
		else if (WorldLocal ==1)
		{
			ScaleX = activeModel->getScaleVector_local().x;
			ScaleY = activeModel->getScaleVector_local().y;
			ScaleZ = activeModel->getScaleVector_local().z;
			ScaleU = activeModel->getScaleVector_local().x;
			RotateX = activeModel->getRotateVector_local().x;
			RotateY = activeModel->getRotateVector_local().y;
			RotateZ = activeModel->getRotateVector_local().z;
			TranslateX = activeModel->getTranslateVector_local().x;
			TranslateY = activeModel->getTranslateVector_local().y;
			TranslateZ = activeModel->getTranslateVector_local().z;
		}

	}

	if (ImGui::RadioButton("World", &WorldLocal, 0))
	{
		ScaleX = activeModel->getScaleVector_world().x;
		ScaleY = activeModel->getScaleVector_world().y;
		ScaleZ = activeModel->getScaleVector_world().z;
		ScaleU = activeModel->getScaleVector_world().x;
		RotateX = activeModel->getRotateVector_world().x;
		RotateY = activeModel->getRotateVector_world().y;
		RotateZ = activeModel->getRotateVector_world().z;
		TranslateX = activeModel->getTranslateVector_world().x;
		TranslateY = activeModel->getTranslateVector_world().y;
		TranslateZ = activeModel->getTranslateVector_world().z;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Local", &WorldLocal, 1))
	{
		ScaleX = activeModel->getScaleVector_local().x;
		ScaleY = activeModel->getScaleVector_local().y;
		ScaleZ = activeModel->getScaleVector_local().z;
		ScaleU = activeModel->getScaleVector_local().x;
		RotateX = activeModel->getRotateVector_local().x;
		RotateY = activeModel->getRotateVector_local().y;
		RotateZ = activeModel->getRotateVector_local().z;
		TranslateX = activeModel->getTranslateVector_local().x;
		TranslateY = activeModel->getTranslateVector_local().y;
		TranslateZ = activeModel->getTranslateVector_local().z;
	}

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Active model : %s", stringToCharSeq(activeModel->GetModelName())); // purple
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.5f, 0.3f, 1.0f, 1.0f), "# of models : %d", scene.GetModelCount());

	ImGui::Checkbox("Scale Model", &show_scale_window);
	ImGui::Checkbox("Rotate Model", &show_rotate_window);
	ImGui::Checkbox("Translate Model", &show_translate_window);
	ImGui::Checkbox("Bounding Box", &bounding_box);
	ImGui::Checkbox("Normals Per Face", &normals_per_face);
	ImGui::SameLine();
	ImGui::Checkbox("Normals Per Vertex", &normals_per_vertex);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();


		ImGui::Begin("Camera Control"); // camera window.

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Active Camera # : %d" , scene.GetActiveCameraIndex());
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.5f, 0.3f, 1.0f, 1.0f), "# of Cameras : %d" , scene.GetCameraCount()); 
		static int currcam = 0;
		Camera& activeCamera = scene.GetCamera(currcam); // getting the active camera. 
		glm::vec3 currEye = activeCamera.getEye();
		glm::vec3 currAt = activeCamera.getAt();
		camX = currEye.x;	camY = currEye.y; disZ = currEye.z;
		atX = currAt.x; atY = currAt.y; atZ = currAt.z;

		ImGui::InputInt("ActiveCam", &currcam);
		if (currcam < 0) currcam = 0;
		if (currcam >= scene.GetCameraCount()) currcam = scene.GetCameraCount() - 1;
		
		if (ImGui::Button("Switch Camera"))
			scene.SetActiveCameraIndex(currcam);

		ImGui::RadioButton("orthographic", &pers, 0);
		ImGui::SameLine();
		ImGui::RadioButton("perspective", &pers, 1);

		ImGui::SliderFloat("Cam X", &camX, -6.0f, 6.0f);
		ImGui::SameLine();
		if (ImGui::Button("Reset camX"))
			camX = 0.0f;
		ImGui::SliderFloat("Cam Y", &camY, -6.0f, 6.0f);
		ImGui::SameLine();
		if (ImGui::Button("Reset camY"))
			camY = 0.0f;
		ImGui::SliderFloat("Dis", &disZ, -6.0f, 6.0f);
		ImGui::SameLine();
		if (ImGui::Button("Reset dist"))
			disZ = 1.0f;
		ImGui::SliderFloat("AT X", &atX, -5.0f, 5.0f );
		ImGui::SameLine();
		if (ImGui::Button("Reset atX"))
			atX = 0.0f;
		ImGui::SliderFloat("AT Y", &atY, -5.0f, 5.0f);
		ImGui::SameLine();
		if (ImGui::Button("Reset atY"))
			atY = 0.0f;
		ImGui::SliderFloat("AT Z", &atZ, -5.0f, 5.0f);
		ImGui::SameLine();
		if (ImGui::Button("Reset atZ"))
			atZ = 0.0f;
		if (pers == 0)
		{
			ImGui::SliderFloat("zoom", &zoom, 1.0f, 5.0f , "%.1f");
			ImGui::SameLine();
			if (ImGui::Button("Reset zoom"))
				zoom = 1.0f;
			activeCamera.setZoom(zoom);
		}
		if (pers == 1)
		{
			ImGui::SliderFloat("fovy", &fovy, 0.0f, 180.0f);
			ImGui::SameLine();
			if (ImGui::Button("Reset fovy"))
				fovy = 50.0f;
			activeCamera.setFOVY(fovy);
		}
		
		//if (ImGui::Button("Look At")) {
		//	glm::mat4 wt = activeModel->getWorldTransformation();
		//	glm::mat4 lt = activeModel->getLocalTransformation();
		//	glm::vec4 lookPoint = glm::vec4({ activeModel->getModelCenter(),1.0f });
		//	lookPoint = wt * lt * lookPoint;
		//	lookPoint = lookPoint / lookPoint.w;
		//	atX = lookPoint.x;
		//	atY = lookPoint.y;
		//	atZ = lookPoint.z;

		//}

		if (currAt != glm::vec3(atX, atY, atZ) || currEye != glm::vec3(camX, camY, disZ)) {
			activeCamera.setCameraLookAt(glm::vec3(camX, camY, disZ), glm::vec3(atX, atY, atZ), glm::vec3(0, 1, 0));
		}

		ImGui::End(); // camera window end.

	if (show_scale_window)
	{
		ImGui::Begin("Scale Window", &show_scale_window);  
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Scaling Values From 0-120");
		ImGui::SliderFloat("Scale X", &ScaleX, 0.0f, 120.0f);      
		ImGui::SameLine();
		if (ImGui::Button("Reset X"))
			ScaleX = 1.0f;
		ImGui::SliderFloat("Scale Y", &ScaleY, 0.0f, 120.0f);  
		ImGui::SameLine();
		if (ImGui::Button("Reset Y"))
			ScaleY = 1.0f;
		ImGui::SliderFloat("Scale Z", &ScaleZ, 0.0f, 120.0f); 
		ImGui::SameLine();
		if (ImGui::Button("Reset Z"))
			ScaleZ = 1.0f;
		ImGui::SliderFloat("Scale U", &ScaleU, 0.0f, 120.0f);
		ImGui::SameLine();
		if (ImGui::Button("Reset U"))
		{
			ScaleU = 1.0f;
			ScaleX = 1.0f;
			ScaleY = 1.0f;
			ScaleZ = 1.0f;
		}

		if (WorldLocal == 0 && (activeModel->getScaleVector_world() != glm::vec3(ScaleX, ScaleY, ScaleZ) || activeModel->getScaleVector_world() != glm::vec3(ScaleU, ScaleU, ScaleU))) // world
		{
		/*	ScaleX = activeModel->getScaleVector_world().x;
			ScaleY = activeModel->getScaleVector_world().y;
			ScaleZ = activeModel->getScaleVector_world().z;*/
			activeModel->setScale(glm::vec3(ScaleX, ScaleY, ScaleZ));
			if (ScaleU != 1)
			{
				ScaleX = ScaleY = ScaleZ = ScaleU;
				activeModel->setScale(ScaleU);
			}
		}
		else if (WorldLocal == 1 && (activeModel->getScaleVector_local() != glm::vec3(ScaleX, ScaleY, ScaleZ) || activeModel->getScaleVector_local() != glm::vec3(ScaleU, ScaleU, ScaleU))) // local
		{
	/*		ScaleX = activeModel->getScaleVector_local().x;
			ScaleY = activeModel->getScaleVector_local().y;
			ScaleZ = activeModel->getScaleVector_local().z;*/
			activeModel->setScale_local(glm::vec3(ScaleX, ScaleY, ScaleZ));
			if (ScaleU != 1)
			{
				ScaleX = ScaleY = ScaleZ = ScaleU;
				activeModel->setScale_local(ScaleU);
			}
		}
		if (ImGui::Button("Close"))
			show_scale_window = false;
		ImGui::End();
	} // end show scale window
	if (show_rotate_window)
	{
		ImGui::Begin("Rotate Window", &show_rotate_window);   
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Rotation Values From -180 to 180"); // yellow
		ImGui::SliderFloat("Rotate X", &RotateX, -180.0f, 180.0f);   
		ImGui::SameLine();
		if (ImGui::Button("Reset X"))
			RotateX = 0.0f;
		ImGui::SliderFloat("Rotate Y", &RotateY, -180.0f, 180.0f);           
		ImGui::SameLine();
		if (ImGui::Button("Reset Y"))
			RotateY = 0.0f;
		ImGui::SliderFloat("Rotate Z", &RotateZ, -180.0f, 180.0f);  
		ImGui::SameLine();
		if (ImGui::Button("Reset Z"))
			RotateZ = 0.0f;

		if (WorldLocal == 0 && activeModel->getRotateVector_world() != glm::vec3(RotateX, RotateY, RotateZ))
		{
			activeModel->setRotate(glm::vec3(RotateX, RotateY, RotateZ));
		}
		else if (WorldLocal == 1 && activeModel->getRotateVector_local() != glm::vec3(RotateX, RotateY, RotateZ))
		{
			activeModel->setRotate_local(glm::vec3(RotateX, RotateY, RotateZ));
		}
		if (ImGui::Button("Close"))
			show_scale_window = false;
		ImGui::End();
	} // end show rotate window
	if (show_translate_window)
	{
		ImGui::Begin("Translate Window", &show_translate_window);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Translating Values From -600 to 600");
		ImGui::SliderFloat("Translate X", &TranslateX, -600.0f, 600.0f);   
		ImGui::SameLine();
		if (ImGui::Button("Reset X"))
			TranslateX = 0.0f;
		ImGui::SliderFloat("Translate Y", &TranslateY, -600.0f, 600.0f);
		ImGui::SameLine();
		if (ImGui::Button("Reset Y"))
			TranslateY = 0.0f;
		ImGui::SliderFloat("Translate Z", &TranslateZ, -600.0f, 600.0f); 
		ImGui::SameLine();
		if (ImGui::Button("Reset Z"))
			TranslateZ = 0.0f;
		if (WorldLocal == 0 && activeModel->getTranslateVector_world() != glm::vec3(TranslateX, TranslateY, TranslateZ))
		{
			activeModel->setTranslate(glm::vec3(TranslateX, TranslateY, TranslateZ));
		}
		else if (WorldLocal == 1 && activeModel->getTranslateVector_local() != glm::vec3(TranslateX, TranslateY, TranslateZ))
		{
			activeModel->setTranslate_local(glm::vec3(TranslateX, TranslateY, TranslateZ));
		}
		if (ImGui::Button("Close"))
			show_scale_window = false;
		ImGui::End();
	} // end show translate window

	if (bounding_box)
	{
		scene.activeBoundBox = true;
	}
	else
	{
		scene.activeBoundBox = false;
	}
	if (normals_per_face)
	{
		scene.activeFacesNormals = true;
	}
	else
	{
		scene.activeFacesNormals = false;
	}
	if (normals_per_vertex)
	{
		scene.activeVerticesNormals = true;
	}
	else
	{
		scene.activeVerticesNormals = false;
	}
	if (pers == 0) // orthographic
	{
		activeCamera.setOrthographicProjection();
	}
	else if(pers == 1) // perspective
	{
		activeCamera.setPerspectiveProjection_Alter();
	}
}