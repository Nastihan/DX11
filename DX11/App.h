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

	Model goblin{ wnd.Gfx(),"Models\\gobber\\GoblinX.obj",6.0f };
	Model wall{ wnd.Gfx(),"Models\\brick_wall\\brick_wall.obj",6.0f };
	TestPlane tp{ wnd.Gfx(),1.0 };
	Model nano{ wnd.Gfx(),"Models\\nano_textured\\nanosuit.obj",2.0f };
};