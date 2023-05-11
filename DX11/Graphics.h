#pragma once
#include "NastihanWin.h"
#include <d3d11.h>

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void ClearBuffer();
	void EndFrame();

private:
	ID3D11Device* device = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11DeviceContext* context = nullptr;
	ID3D11RenderTargetView* targetView = nullptr;
};