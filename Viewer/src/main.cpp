#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"
#include "GuiMenu.h"

/**
 * Fields
 */
extern glm::vec4 clear_color1;


extern float ScaleU;

extern float RotateX;
extern float RotateY;
extern float RotateZ;

extern float TranslateX;
extern float TranslateY;
extern float TranslateZ;

extern float zoom;

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char **argv)
{
	int windowWidth = 1280, windowHeight = 720;
	// creating window
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;
	// get current frame width&height from buffer
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	// creates rederer and scene
	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
	// setting up ImGui
	ImGuiIO& io = SetupDearImgui(window);
	// handles scroll-wheel
	glfwSetScrollCallback(window, ScrollCallback);
	// adding camera to scene
	scene.AddCamera((std::shared_ptr<Camera>) &Camera(glm::vec3{ 0,0,1 }, glm::vec3{ 0,0,0 }, glm::vec3{ 0,1,0 }));
	scene.SetActiveCameraIndex(0);

	// main Loop
	while (!glfwWindowShouldClose(window))
    {
		glfwPollEvents();
		StartFrame();
		// creating the User Interface
		DrawImguiMenus(io, scene);
		// renders next frame
		RenderFrame(window, scene, renderer, io);
    }
	// done using program
	Cleanup(window);
    return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	
	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	// renders menus
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	
	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		// TODO: Set new aspect ratio
		if (frameBufferHeight != 0) {
			scene.GetActiveCamera().setAspectRatio((float)frameBufferWidth / (float)frameBufferHeight);
		}
		renderer.SetViewport(frameBufferWidth, frameBufferHeight);
	}
	// Use the ASCII table for more key codes (https://www.asciitable.com/)
	if (!io.WantCaptureKeyboard)
	{
		if (io.KeysDown[87]) 	TranslateY++;  // W
		if (io.KeysDown[83])	TranslateY--; // S
		if (io.KeysDown[65])    TranslateX--; // A
		if (io.KeysDown[68])    TranslateX++; // D
		if (io.KeysDown[90])    TranslateZ--; // Z
		if (io.KeysDown[67])    TranslateZ++; // C
		if (io.KeysDown[82])    ScaleU++; // R
		if (io.KeysDown[70])    ScaleU--; // F
		if (io.KeysDown[89])    RotateY++; // Y
		if (io.KeysDown[72])    RotateY--; // H
		if (io.KeysDown[71])    RotateX--; // G
		if (io.KeysDown[74])    RotateX++; // J
		if (io.KeysDown[66])    RotateZ--; // B
		if (io.KeysDown[77])    RotateZ++; // M
	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0]) // Left mouse button is down
		{
			if (io.MouseDelta.y < 0)
			{
				zoom = zoom + 0.02f;
			}
			else if (io.MouseDelta.y > 0)
			{
				zoom = zoom - 0.02f;
			}
		}
		if (io.MouseDown[1])
		{
			if (io.MouseDelta.y < 0)
			{
				scene.activeFacesNormals = true;
			}
			if (io.MouseDelta.x < 0)
			{
				scene.activeVerticesNormals = true;
			}
		}
		//activeVerticesNormals
		if (io.MouseDoubleClicked[0]) 
		{
			
		}
	}
	// clears frame buffer
	renderer.ClearColorBuffer(clear_color1); // clear_color1
	// Renders Scene
	renderer.Render(scene);
	// swap buffers
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
}

