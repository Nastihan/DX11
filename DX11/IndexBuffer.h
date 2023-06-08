#pragma once
#include "Bindable.h"

namespace Bind
{
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(Graphics& gfx, std::vector<unsigned short> indiices);
		void Bind(Graphics& gfx) noexcept override;
		UINT GetCount() const noexcept;
	protected:
		UINT count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	};
}