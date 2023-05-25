#include "Sampler.h"
#include "GraphicsThrowMacros.h"

Sampler::Sampler(Graphics& gfx)
{
	INFOMAN(gfx);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSamplerSate));
}

void Sampler::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetSamplers(0u, 1u, pSamplerSate.GetAddressOf());
}
