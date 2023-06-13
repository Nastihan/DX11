#pragma once
#include "Window.h"
#include "NastihanTimer.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include <set>
#include "Model.h"
#include "TestPlane.h"


class App
{
public:
	App();
	~App();
	int Go();
private:
	void DoFrame();
	void ShowHelperWindow();
	void ShowFPSWindow();
private:
	float speedFactor = 1.0f;
	bool showDemoWindow = true;
	ImGuiManager imgui;
	Window wnd;
	NastihanTimer timer;
	Camera cam;
	PointLight light;

	TestPlane plane;
	Model nano{ wnd.Gfx(),"Models\\nano_textured\\nanosuit.obj" };
};