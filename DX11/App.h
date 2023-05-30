#pragma once
#include "Window.h"
#include "NastihanTimer.h"
#include "ImGuiManager.h"
#include "Camera.h"




class App
{
public:
	App();
	~App();
	int Go();
	void DoFrame();

private:
	float speedFactor = 1.0f;
	bool showDemoWindow = true;
	ImGuiManager imgui;
	Window wnd;
	NastihanTimer timer;
	Camera cam;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawables = 280;
};