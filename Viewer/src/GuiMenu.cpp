#pragma once
#include "GuiMenu.h"
#include <stdio.h>
#include <stdlib.h>
#include <nfd.h>
#include "Scene.h"
#include "Utils.h"

/**
 * Fields
 */

bool show_demo_window = false;
bool show_scale_window = false;
bool show_rotate_window = false;
bool show_translate_window = false;

glm::vec4 clear_color1 = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
glm::vec3 ObjColor = glm::vec3(1.0f, 0.0f, 1.0f);

static float ScaleX = 1.0f;
static float ScaleY = 1.0f;
static float ScaleZ = 1.0f;

static float RotateX = 0.0f;
static float RotateY = 0.0f;
static float RotateZ = 0.0f;

static float TranslateX = 0.0f;
static float TranslateY = 0.0f;
static float TranslateZ = 0.0f;

/**
 * Function implementation
 */

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

	{
	

		static int counter = 0;

		ImGui::Begin("Model Control");                          // Create a window called "Model Contorl" and append into it.

	
		ImGui::Checkbox("Scale Model", &show_scale_window);
		ImGui::Checkbox("Rotate Model", &show_rotate_window);
		ImGui::Checkbox("Translate Model", &show_translate_window);

		ImGui::ColorEdit3("clear color", (float*)&clear_color1); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_scale_window)
	{
		ImGui::Begin("Scale Window", &show_scale_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Scale Values From 0-10");
		ImGui::SliderFloat("Scale X", &ScaleX, 0.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("Scale Y", &ScaleY, 0.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("Scale Z", &ScaleZ, 0.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		if (ImGui::Button("Close"))
			show_scale_window = false;
		ImGui::End();
	}
}