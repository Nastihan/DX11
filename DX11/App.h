#pragma once
#include "Window.h"
#include "NastihanTimer.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include <set>
#include "Model.h"



class App
{
public:
	App();
	~App();
	int Go();
private:
	void DoFrame();
	void SpawnSimulationWindow() noexcept;
private:
	float speedFactor = 1.0f;
	bool showDemoWindow = true;
	ImGuiManager imgui;
	Window wnd;
	NastihanTimer timer;
	Camera cam;
	PointLight light;


	Model nano{ wnd.Gfx(),"Models\\nano.gltf" };
};