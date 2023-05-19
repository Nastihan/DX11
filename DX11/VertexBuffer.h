#pragma once
#include "Bindable.h"
#include "GraphicsThrowMacros.h"


class VertexBuffer : Bindable
{
public:
	template <typename V>
	VertexBuffer(Graphics& gfx, const std::vector<V> vertices);
	void Bind(Graphics& gfx) noexcept override;
protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};

