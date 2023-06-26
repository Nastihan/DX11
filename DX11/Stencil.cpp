#include "Stencil.h"


namespace Bind
{
	Stencil::Stencil(Graphics& gfx)
	{
		D3D11_DEPTH_STENCIL_DESC stencilDesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT{});
		stencilDesc.StencilEnable = true;
		stencilDesc.StencilWriteMask = 0xff;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		GetDevice(gfx)->CreateDepthStencilState(&stencilDesc, pStencil.GetAddressOf());
	}
	void Stencil::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->OMSetDepthStencilState(pStencil.Get(), 0u);
	}
}