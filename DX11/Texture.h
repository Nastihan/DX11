#pragma once
#include "Bindable.h"

class Surface;

namespace Bind
{
	class Texture : public Bindable
	{
	public:
		Texture(Graphics& gfx, const class Surface& surface,UINT slot = 0u);
		void Bind(Graphics& gfx) noexcept override;
	private:
		UINT slot = 0;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;

	};
}