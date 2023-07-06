#include "DepthStencil.h"
#include "GraphicsThrowMacros.h"

DepthStencil::DepthStencil(Graphics& gfx, UINT width,UINT height)
{
	INFOMAN(gfx);

	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1u;
	textureDesc.ArraySize = 1u;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.SampleDesc.Count = 1u;
	textureDesc.SampleDesc.Quality = 0u;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&textureDesc, nullptr, &pTexture));


	GetDevice(gfx)->CreateDepthStencilView(pTexture.Get(), nullptr, &pDepthStencilView);
}

void DepthStencil::BindAsDepthStencil(Graphics& gfx) const noexcept
{
	GetContext(gfx)->OMSetRenderTargets(0, nullptr, pDepthStencilView.Get());
}

void DepthStencil::Clear(Graphics& gfx) const noexcept
{
	GetContext(gfx)->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
}
