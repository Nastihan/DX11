#pragma once
#include "Window.h"
#include "NastihanTimer.h"


class App
{
public:
	App();
	~App();
	int Go();
	void DoFrame();

private:
	Window wnd;
	NastihanTimer timer;
	std::vector<std::unique_ptr<class Box>> boxes;
};