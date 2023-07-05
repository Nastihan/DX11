#pragma once
#include "Window.h"
#include "NastihanTimer.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include <set>
#include "TestPlane.h"
#include "TestCube.h"
#include "ScriptCommander.h"
#include "FrameCommander.h"
#include "Model.h"

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
	ScriptCommander scriptCommander;
	NastihanTimer timer;
	Camera cam;
	PointLight light;
	FrameCommander fc;


	TestCube cube1{ wnd.Gfx(),3.6f };
	TestCube cube2{ wnd.Gfx(),4.2f };
	//Model goblin{ wnd.Gfx(),"Models\\gobber\\GoblinX.obj",6.0f };
	Model sponza{ wnd.Gfx(),"Models\\Sponza\\sponza.obj",1/20.0f };
};