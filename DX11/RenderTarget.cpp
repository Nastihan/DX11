#include "RenderTarget.h"
#include "GraphicsThrowMacros.h"
#include "DepthStencil.h"
#include <array>

RenderTarget::RenderTarget(Graphics& gfx, UINT width, UINT Height)
{
	INFOMAN(gfx);

	D3D11_TEXTURE2D_DESC tDesc{};
	tDesc.Width = width;
	tDesc.Height = Height;
	tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.MipLevels = 1;
	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	tDesc.CPUAccessFlags = 0;
	tDesc.MiscFlags = 0;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;

	GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&tDesc, nullptr, &pTexture));


	// create the resource view on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = tDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(
		pTexture.Get(), &srvDesc, &pTextureView
	));

	// create the target view on the texture
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = tDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	GFX_THROW_INFO(GetDevice(gfx)->CreateRenderTargetView(
		pTexture.Get(), &rtvDesc, &pTargetView
	));
}

void RenderTarget::BindAsTexture(Graphics& gfx,UINT slot) const noexcept
{
	GetContext(gfx)->PSSetShaderResources(slot, 1, pTextureView.GetAddressOf());
}

void RenderTarget::BindAsTarget(Graphics& gfx) const noexcept
{
	GetContext(gfx)->OMSetRenderTargets(1, pTargetView.GetAddressOf(), nullptr);
}

void RenderTarget::BindAsTarget(Graphics& gfx,const DepthStencil& ds) const noexcept
{
	GetContext(gfx)->OMSetRenderTargets(1, pTargetView.GetAddressOf(), ds.pDepthStencilView.Get());
}

void RenderTarget::Clear(Graphics& gfx, const std::array<float, 4>& color) const noexcept
{
	GetContext(gfx)->ClearRenderTargetView(pTargetView.Get(), color.data());
}

void RenderTarget::Clear(Graphics& gfx) const noexcept
{
	Clear(gfx, { 0.0f,0.0f,0.0f,0.0f });
}