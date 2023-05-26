#include "Sampler.h"
#include "GraphicsThrowMacros.h"

Sampler::Sampler(Graphics& gfx)
{
	INFOMAN(gfx);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	float ColorArray[4] = { 0.2f, 0.7f, 0.4f, 1.0f };
	samplerDesc.BorderColor[0] = ColorArray[0];
	samplerDesc.BorderColor[1] = ColorArray[1];
	samplerDesc.BorderColor[2] = ColorArray[2];
	samplerDesc.BorderColor[3] = ColorArray[3];

	GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSamplerSate));
}

void Sampler::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetSamplers(0u, 1u, pSamplerSate.GetAddressOf());
}
