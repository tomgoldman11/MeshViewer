#pragma once
#include "GuiMenu.h"
#include <stdio.h>
#include <stdlib.h>
#include <nfd.h>
#include "Scene.h"
#include "Utils.h"
#include "MeshModel.h"

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
bool pers = false;


glm::vec4 clear_color1 = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
glm::vec3 ObjColor = glm::vec3(1.0f, 0.0f, 1.0f);

 static float ScaleX = 1.0f;
 static float ScaleY = 1.0f;
 static float ScaleZ = 1.0f;

 static float RotateX = 101.0f;
 static float RotateY = 5.0f;
 static float RotateZ = -45.0f;

static float TranslateX = 0.0f;
static float TranslateY = 0.0f;
static float TranslateZ = 0.0f;

static float camX = 0.0f;
static float camY = 0.0f;
static float disZ = -1.0f;


/**
 * Function implementation
 */

const glm::vec4& GetClearColor()
{
	return clear_color1;
}


void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	ImGui::SetNextWindowPos(ImVec2(0, 30), ImGuiCond_Once);
	/**
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
				scene.AddCamera((std::shared_ptr<Camera>) &Camera(glm::vec3{ 0,0,1 }, glm::vec3{ 0,0,0 }, glm::vec3{ 0,1,0 }));
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
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
	ImGui::ColorEdit3("Clear Color", (float*)&clear_color1);
	// TODO: Add more controls as needed

	ImGui::End();

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);


	static int counter = 0;
	ImGui::Begin("Model Control");                          // Create a window called "Model Contorl" and append into it.

															
	if (scene.GetModelCount() == 0)
	{
		ImGui::Text("No models loaded yet");
		ImGui::End();
		return;
	}


	ImGui::Checkbox("Scale Model", &show_scale_window);
	ImGui::Checkbox("Rotate Model", &show_rotate_window);
	ImGui::Checkbox("Translate Model", &show_translate_window);

	ImGui::Checkbox("Bounding Box", &bounding_box);
	ImGui::Checkbox("Normals Per Face", &normals_per_face);
	ImGui::SameLine();
	ImGui::Checkbox("Normals Per Vertex", &normals_per_vertex);

	ImGui::ColorEdit3("clear color", (float*)&clear_color1); // Edit 3 floats representing a color

		//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	counter++;
		//ImGui::SameLine();
		//ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		static Camera& activeCamera = scene.GetActiveCamera();

		ImGui::Begin("Camera Control"); // camera window.

		ImGui::Checkbox("orthographic", &ortho);
		ImGui::SameLine();
		ImGui::Checkbox("perspective", &pers);

		ImGui::SliderFloat("Cam X", &camX, -6.0f, 6.0f);
		ImGui::SliderFloat("Cam Y", &camY, -6.0f, 6.0f);
		ImGui::SliderFloat("Distance", &disZ, -1.0f, 1.0f);

		activeCamera.setCameraLookAt(glm::vec3(camX, camY, disZ), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		


		ImGui::End(); // camera window end.

		static MeshModel& activeModel = scene.GetActiveModel(); // getting the current model.
	
		

	if (show_scale_window)
	{
		ImGui::Begin("Scale Window", &show_scale_window);  
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Scaling Values From 0-20");
		ImGui::SliderFloat("Scale X", &ScaleX, 0.0f, 20.0f);      
		ImGui::SameLine();
		if (ImGui::Button("Reset X"))
			ScaleX = 1.0f;
		ImGui::SliderFloat("Scale Y", &ScaleY, 0.0f, 20.0f);  
		ImGui::SameLine();
		if (ImGui::Button("Reset Y"))
			ScaleY = 1.0f;
		ImGui::SliderFloat("Scale Z", &ScaleZ, 0.0f, 20.0f); 
		ImGui::SameLine();
		if (ImGui::Button("Reset Z"))
			ScaleZ = 1.0f;

		activeModel.setScale(glm::vec3(ScaleX, ScaleY, ScaleZ));

		if (ImGui::Button("Close"))
			show_scale_window = false;
		ImGui::End();
	}
	if (show_rotate_window)
	{
		ImGui::Begin("Rotate Window", &show_rotate_window);   
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Rotation Values From -180 to 180"); // yellow
		//ImGui::TextColored(ImVec4(0.5f, 0.3f, 1.0f, 1.0f), "Rotation Values From -180 to 180"); // purple
		ImGui::SliderFloat("Rotate X", &RotateX, -180.0f, 180.0f);   
		ImGui::SameLine();
		if (ImGui::Button("Reset X"))
			RotateX = 101.0f;
		ImGui::SliderFloat("Rotate Y", &RotateY, -180.0f, 180.0f);           
		ImGui::SameLine();
		if (ImGui::Button("Reset Y"))
			RotateY = 5.0f;
		ImGui::SliderFloat("Rotate Z", &RotateZ, -180.0f, 180.0f);  
		ImGui::SameLine();
		if (ImGui::Button("Reset Z"))
			RotateZ = -45.0f;

		activeModel.setRotate(glm::vec3(RotateX, RotateY, RotateZ));

		if (ImGui::Button("Close"))
			show_scale_window = false;
		ImGui::End();
	}
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

		activeModel.setTranslate(glm::vec3(TranslateX, TranslateY, TranslateZ));

		if (ImGui::Button("Close"))
			show_scale_window = false;
		ImGui::End();
	}
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
	if (ortho)
	{

	}
	if (pers)
	{

	}
}