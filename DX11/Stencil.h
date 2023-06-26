#pragma once
#include "Bindable.h"



namespace Bind
{
	class Stencil : public Bindable
	{
	public:
		Stencil(Graphics& gfx);
		void Bind(Graphics& gfx) noexcept override;


	private:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pStencil;
	};
}