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
	App(const std::string& commandLine = "");
	~App();
	int Go();
private:
	void DoFrame();
	void ShowHelperWindow();
	void ShowFPSWindow();
private:
	std::string commandLine;
	float speedFactor = 1.0f;
	bool showDemoWindow = true;
	ImGuiManager imgui;
	Window wnd;
	NastihanTimer timer;
	Camera cam;
	PointLight light;

	Model sponza{ wnd.Gfx(),"Models\\Sponza\\sponza.obj",1/20.0f };
};