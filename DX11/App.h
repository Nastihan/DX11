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
	void SpawnModelControlWindow() noexcept;
private:
	float speedFactor = 1.0f;
	bool showDemoWindow = true;
	ImGuiManager imgui;
	Window wnd;
	NastihanTimer timer;
	Camera cam;
	PointLight light;

	struct transformation
	{
		float roll;
		float pitch;
		float yaw;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	}model;

	Model nano{ wnd.Gfx(),"Models\\nanosuit.obj" };
};