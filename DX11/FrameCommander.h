#pragma once
#include <array>
#include "BindableCommon.h"
#include "Graphics.h"
#include "Job.h"
#include "Pass.h"
#include "PerfLog.h"
#include "DepthStencil.h"
#include "RenderTarget.h"

class FrameCommander
{
public:
	FrameCommander(Graphics& gfx)
		:
		ds(gfx, gfx.GetWidth(), gfx.GetHeight()),
		rt(gfx,gfx.GetWidth(),gfx.GetHeight())
	{
		// fullscreen quad vertexbuffer 
		Dvtx::VertexLayout lay{};
		lay.Append(Dvtx::VertexLayout::ElementType::Position2D);
		Dvtx::VertexBuffer buf{lay};
		buf.EmplaceBack(DirectX::XMFLOAT2{-1.0, 1.0});
		buf.EmplaceBack(DirectX::XMFLOAT2{1.0, 1.0});
		buf.EmplaceBack(DirectX::XMFLOAT2{-1.0, -1.0});
		buf.EmplaceBack(DirectX::XMFLOAT2{1.0, -1.0});
		pVertexBuffer = Bind::VertexBuffer::Resolve(gfx,"FULLSCREEN", std::move(buf));
		
		// fullscreen quad index buffer
		std::vector <unsigned short> indices{0, 1, 2, 1, 3, 2};
		pIndexBuffer = Bind::IndexBuffer::Resolve(gfx, "FULLSCREEN", indices);
		
		// fullscreen quad VS shader
		pVS = Bind::VertexShader::Resolve(gfx, "Fullscreen_VS.cso");
		auto bc = pVS->GetBytecode();

		// fullscreen quad input layout
		pInputLayout = Bind::InputLayout::Resolve(gfx, lay, bc);

		// fullscreen quad PS shader
		pPS = Bind::PixelShader::Resolve(gfx, "Negative_PS.cso");

	}
	void Accept(Job job, size_t target) noexcept
	{
		passes[target].Accept(job);
	}
	void Execute(Graphics& gfx) const noxnd
	{
		using namespace Bind;
		// normally this would be a loop with each pass defining it setup / etc.
		// and later on it would be a complex graph with parallel execution contingent
		// on input / output requirements

		// clear z buffer
		ds.Clear(gfx);
		// setup render target and z buffer for all calls
		rt.BindAsTarget(gfx,ds);
		//gfx.BindSwapBuffer(ds);


		// main phong lighting pass
		Stencil::Resolve(gfx, Stencil::Mode::Off)->Bind(gfx);
		passes[0].Execute(gfx);
		// outline masking pass
		Stencil::Resolve(gfx, Stencil::Mode::Write)->Bind(gfx);
		NullPixelShader::Resolve(gfx)->Bind(gfx);
		passes[1].Execute(gfx);
		//// outline drawing pass
		Stencil::Resolve(gfx, Stencil::Mode::Mask)->Bind(gfx);
		passes[2].Execute(gfx);

		// full screen effect
		gfx.BindSwapBuffer(ds);
		rt.BindAsTexture(gfx, 0);

		pVertexBuffer->Bind(gfx);
		pIndexBuffer->Bind(gfx);
		pInputLayout->Bind(gfx);
		pVS->Bind(gfx);
		pPS->Bind(gfx);
		gfx.DrawIndexed(pIndexBuffer->GetCount());
		
	}
	void Reset() noexcept
	{
		for (auto& p : passes)
		{
			p.Reset();
		}
	}
private:
	DepthStencil ds;
	RenderTarget rt;
	std::array<Pass, 3> passes;

	std::shared_ptr<Bind::VertexBuffer> pVertexBuffer;
	std::shared_ptr<Bind::IndexBuffer> pIndexBuffer;
	std::shared_ptr<Bind::InputLayout> pInputLayout;
	std::shared_ptr < Bind::VertexShader> pVS;
	std::shared_ptr<Bind::PixelShader> pPS;

};