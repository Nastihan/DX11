#pragma once
#include <array>
#include "BindableCommon.h"
#include "Graphics.h"
#include "Job.h"
#include "Pass.h"
#include "PerfLog.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "BlurPack.h"

class FrameCommander
{
public:
	FrameCommander(Graphics& gfx)
		:
		ds(gfx, gfx.GetWidth(), gfx.GetHeight()),
		rt1(gfx,gfx.GetWidth(),gfx.GetHeight()),
		rt2(gfx,gfx.GetWidth(),gfx.GetHeight()),
		blur(gfx)
	{
		// fullscreen quad vertexbuffer 
		Dvtx::VertexLayout lay{};
		lay.Append(Dvtx::VertexLayout::ElementType::Position2D);
		Dvtx::VertexBuffer buf{lay};
		buf.EmplaceBack(DirectX::XMFLOAT2{-1.0, 1.0});
		buf.EmplaceBack(DirectX::XMFLOAT2{1.0, 1.0});
		buf.EmplaceBack(DirectX::XMFLOAT2{-1.0, -1.0});
		buf.EmplaceBack(DirectX::XMFLOAT2{1.0, -1.0});
		pVertexBuffer = Bind::VertexBuffer::Resolve(gfx,"$Full", std::move(buf));
		// fullscreen quad index buffer
		std::vector<unsigned short> indices = { 0,1,2,1,3,2 };
		pIndexBuffer = Bind::IndexBuffer::Resolve(gfx, "$Full", std::move(indices));

		// fullscreen quad stuff
		pVS = Bind::VertexShader::Resolve(gfx, "Fullscreen_VS.cso");
		pInputLayout = Bind::InputLayout::Resolve(gfx, lay, pVS->GetBytecode());
		pSampler = Bind::Sampler::Resolve(gfx, false, true);
	}
	void Accept(Job job, size_t target) noexcept
	{
		passes[target].Accept(job);
	}
	void Execute(Graphics& gfx) noxnd
	{
		using namespace Bind;
		// normally this would be a loop with each pass defining it setup / etc.
		// and later on it would be a complex graph with parallel execution contingent
		// on input / output requirements

		// clear buffer
		rt1.Clear(gfx);
		ds.Clear(gfx);
		// setup render target and z buffer for all calls
		rt1.BindAsTarget(gfx,ds);

		// main phong lighting pass
		Blender::Resolve(gfx, false)->Bind(gfx);
		Stencil::Resolve(gfx, Stencil::Mode::Off)->Bind(gfx);
		passes[0].Execute(gfx);
		// outline masking pass
		//Stencil::Resolve(gfx, Stencil::Mode::Write)->Bind(gfx);
		//NullPixelShader::Resolve(gfx)->Bind(gfx);
		//passes[1].Execute(gfx);
		//// outline drawing pass
		//rt1.BindAsTarget(gfx);
		//Stencil::Resolve(gfx, Stencil::Mode::Off)->Bind(gfx);
		//passes[2].Execute(gfx);

		// full separated gaussian blur screen effect h pass
		rt2.BindAsTarget(gfx);
		rt1.BindAsTexture(gfx, 0);

		blur.SetHorizontal(gfx);
		blur.Bind(gfx);
		pVertexBuffer->Bind(gfx);
		pIndexBuffer->Bind(gfx);
		pInputLayout->Bind(gfx);
		pVS->Bind(gfx);
		pSampler->Bind(gfx);
		gfx.DrawIndexed(pIndexBuffer->GetCount());

		// v pass
		gfx.BindSwapBuffer(ds);
		rt2.BindAsTexture(gfx, 0);
		blur.SetVertical(gfx);
		gfx.DrawIndexed(pIndexBuffer->GetCount());


	}
	void ShowWindows(Graphics& gfx)
	{
		blur.ShowWindow(gfx);
	}
	void Reset() noexcept
	{
		for (auto& p : passes)
		{
			p.Reset();
		}
	}
private:
	std::array<Pass, 3> passes;

	DepthStencil ds;
	RenderTarget rt1;
	RenderTarget rt2;
	BlurPack blur;

	std::shared_ptr<Bind::VertexBuffer> pVertexBuffer;
	std::shared_ptr<Bind::IndexBuffer> pIndexBuffer;
	std::shared_ptr<Bind::InputLayout> pInputLayout;
	std::shared_ptr < Bind::VertexShader> pVS;
	std::shared_ptr<Bind::Sampler> pSampler;
};