#include "VertexBuffer.h"


template<typename V>
inline VertexBuffer::VertexBuffer(Graphics& gfx, const std::vector<V> vertices)
	: stride(sizeof(V)){
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = UINT(sizeof(V) * vertices.size());
	bufferDesc.StructureByteStride = sizeof(V);
	D3D11_SUBRESOURCE_DATA sData;
	sData.pSysMem = vertices.data();
	GetDevice(gfx)->CreateBuffer(&bufferDesc, &sData, &pVertexBuffer);
}

void VertexBuffer::Bind(Graphics& gfx) noexcept
{
	const UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}
