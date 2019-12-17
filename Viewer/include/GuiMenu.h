#pragma once
#include <imgui/imgui.h>
#include "Scene.h"



const glm::vec4& GetClearColor();
void DrawImguiMenus(ImGuiIO& io, Scene& scene);
char* const stringTocharSeq(const std::string& str);
