#pragma once
#include "Window.h"
#include "NastihanTimer.h"
#include "ImGuiManager.h"



class App
{
public:
	App();
	~App();
	int Go();
	void DoFrame();

private:
	ImGuiManager imgui;
	Window wnd;
	NastihanTimer timer;
	std::vector<std::unique_ptr<class Pyramid>> pyramids;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawables = 280;
};