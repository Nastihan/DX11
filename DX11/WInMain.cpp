#include "Window.h"



int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	try {
		Window wnd(800, 300, "DX11");

		// message
		MSG msg;
		BOOL gResult;
		while (gResult = GetMessage(&msg, nullptr, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
				
			
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
	catch (const NastihanException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}