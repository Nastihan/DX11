#include "Rasterizer.h"
#include "BindableCodex.h"
#include "GraphicsThrowMacros.h"

namespace Bind
{
	Bind::Rasterizer::Rasterizer(Graphics& gfx,bool twoSided)
	{
		INFOMAN(gfx);

		D3D11_RASTERIZER_DESC rDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		rDesc.CullMode = twoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK;
		GetDevice(gfx)->CreateRasterizerState(&rDesc, pRasterizer.GetAddressOf());
	}

	void Bind::Rasterizer::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->RSSetState(pRasterizer.Get());
	}

	std::shared_ptr<Rasterizer> Rasterizer::Resolve(Graphics& gfx ,bool twoSided)
	{
		return Codex::Resolve<Rasterizer>(gfx,twoSided);
	}
	std::string Rasterizer::GenerateUID(bool twoSided)
	{
		using namespace std::string_literals;
		return typeid(Rasterizer).name() + "#"s + (twoSided ? "2s" : "1s");
	}
	std::string Rasterizer::GetUID() const noexcept
	{
		return GenerateUID(twoSided);
	}
}