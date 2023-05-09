#pragma once
#include "Window.h"
#include "NastihanTimer.h"


class App
{
public:
	App();
	int Go();
	void DoFrame();

private:
	Window wnd;
	NastihanTimer timer;
};