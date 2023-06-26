#pragma once
#include "Bindable.h"



namespace Bind
{
	class Stencil : public Bindable
	{
	public:
		enum Mode
		{
			Off,
			Write,
			Mask,
		};
		Stencil(Graphics& gfx, Mode mode);
		void Bind(Graphics& gfx) noexcept override;


	private:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pStencil;
	};
}