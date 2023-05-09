#include "App.h"


App::App()
    : wnd (800, 300, "DX11")
{
}

int App::Go()
{
	


	// message
	MSG msg;
	BOOL gResult;
	while (gResult = GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		while (!wnd.mouse.IsEmpty())
		{
			const auto e = wnd.mouse.Read();
			if (e.GetType() == Mouse::Event::Type::Move)
			{
				std::ostringstream oss;
				oss << "Mouse Position: (" << e.GetPosX() << "," << e.GetPosY() << ")";
				wnd.SetTitle(oss.str());
			}

		}


	}

	if (gResult == -1)
	{
		throw CHWND_LAST_EXCEPT();
	}
	else
	{
		return msg.wParam;
	}
}

void App::DoFrame()
{
}
