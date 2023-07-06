#pragma once
#include "GraphicsResource.h"

class RenderTarget : public GraphicsResource
{
public :
	RenderTarget(Graphics& gfx, UINT width, UINT Height);
	void BindAsTexture(Graphics& gfx,UINT slot);
	void BindAsTarget(Graphics& gfx);
	void BindAsTarget(Graphics& gfx, DepthStencil& ds);
private: 
	Microsoft::WRL::ComPtr < ID3D11ShaderResourceView> pTextureView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;
};
