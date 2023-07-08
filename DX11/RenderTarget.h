#pragma once
#include "GraphicsResource.h"

class RenderTarget : public GraphicsResource
{
public:
	RenderTarget(Graphics& gfx, UINT width, UINT Height);
	void BindAsTexture(Graphics& gfx,UINT slot) const noexcept;
	void BindAsTarget(Graphics& gfx) const noexcept;
	void BindAsTarget(Graphics& gfx,const DepthStencil& ds)const noexcept;
	void Clear(Graphics& gfx, const std::array<float, 4>& color) const noexcept;
	void Clear(Graphics& gfx) const noexcept;
private: 
	Microsoft::WRL::ComPtr < ID3D11ShaderResourceView> pTextureView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;
};