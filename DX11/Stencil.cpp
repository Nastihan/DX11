#include "Stencil.h"


namespace Bind
{
	
	Stencil::Stencil(Graphics& gfx ,Mode mode)
	{
		D3D11_DEPTH_STENCIL_DESC stencilDesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT{});

		if (mode == Write)
		{
			stencilDesc.StencilEnable = true;
			stencilDesc.StencilWriteMask = 0xff;
			stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
			stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		}
		if (mode == Mask)
		{
			stencilDesc.DepthEnable = false;
			stencilDesc.StencilEnable = true;
			stencilDesc.StencilWriteMask = 0xff;
			stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		}
		GetDevice(gfx)->CreateDepthStencilState(&stencilDesc, pStencil.GetAddressOf());
	}
	void Stencil::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->OMSetDepthStencilState(pStencil.Get(), 0xff);
	}
}