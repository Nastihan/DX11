#pragma once
#include "Window.h"

class App
{
public:
	App();
	int Go();
	void DoFrame();

private:
	Window wnd;

};