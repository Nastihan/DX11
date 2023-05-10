#pragma once
#include "NastihanWin.h"
#include <d3d11.h>

class Graphics
{
public:
	Graphics(HWND hWnd);

private:
	ID3D11Device* device = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11DeviceContext* context = nullptr;
};