#include "Graphics.h"

#pragma comment(lib,"d3d11.lib")


Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	// Display mode of the backbuffer
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// Multisampling
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	// Backbuffer usage
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// Number of backbuffers
	swapChainDesc.BufferCount = 1;
	// Handle to the window
	swapChainDesc.OutputWindow = hWnd;
	// Set whether the swap chain will be created in windowed mode or full-screen mode
	swapChainDesc.Windowed = TRUE;
	// Set how the contents of the back buffer are treated after a buffer swap
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	////////////////////////
	swapChainDesc.Flags = 0;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&device,
		nullptr,
		&context
	);
	
}
