#pragma once
#include "Window.h"
#include "NastihanTimer.h"
#include "ImguiManager.h"
#include "PointLight.h"
#include "TestCube.h"
#include "Mesh.h"
#include "Model.h"
#include "ScriptCommander.h"
#include "BlurOutlineRenderGraph.h"
#include "NastihanMath.h"
#include "CameraContainer.h"

class App
{
public:
	App(const std::string& commandLine = "");
	int Go();
	~App();
private:
	void DoFrame(float dt);
	void HandleInput(float dt);
	void ShowImguiDemoWindow();
	void ShowFPSWindow();

private:
	std::string commandLine;
	bool showDemoWindow = false;
	ImGuiManager imgui;
	Window wnd;
	ScriptCommander scriptCommander;
	Rgph::BlurOutlineRenderGraph rg{ wnd.Gfx() };
	NastihanTimer timer;
	float speed_factor = 1.0f;
	CameraContainer cameras;
	PointLight light;
	TestCube cube{ wnd.Gfx(),4.0f };
	TestCube cube2{ wnd.Gfx(),4.0f };
	Model sponza{ wnd.Gfx(),"Models\\sponza\\sponza.obj",1.0f / 20.0f };
	Model gobber{ wnd.Gfx(),"Models\\gobber\\GoblinX.obj",4.0f };
	Model nano{ wnd.Gfx(),"Models\\nano_textured\\nanosuit.obj",2.0f };
};