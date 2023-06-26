#include "Stencil.h"


namespace Bind
{
	Stencil::Stencil(Graphics& gfx)
	{
		D3D11_DEPTH_STENCIL_DESC stencilDesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT{});

		GetDevice(gfx)->CreateDepthStencilState(&stencilDesc, pStencil.GetAddressOf());
	}
	void Stencil::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->OMSetDepthStencilState(pStencil.Get(), 0u);
	}
}