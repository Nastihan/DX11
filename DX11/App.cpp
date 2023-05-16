#include "App.h"


App::App()
    : wnd (1000, 800, "DX11")
{
}

int App::Go()
{
	
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}

}

void App::DoFrame()
{
	
	auto alpha = sin(timer.Peek() / 2.0f + 0.5f);
	wnd.Gfx().ClearBuffer();
	wnd.Gfx().DrawTriangle(timer.Peek());
	wnd.Gfx().EndFrame();
}
