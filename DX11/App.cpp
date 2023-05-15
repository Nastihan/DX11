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
	wnd.Gfx().ClearBuffer();
	wnd.Gfx().DrawTriangle();
	wnd.Gfx().EndFrame();
}
