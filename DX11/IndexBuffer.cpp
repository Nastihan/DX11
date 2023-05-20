#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Graphics& gfx, std::vector<unsigned short> indices)
	: count((UINT)indices.size()){

	D3D11_BUFFER_DESC bDesc;
	bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bDesc.Usage = D3D11_USAGE_DEFAULT;
	bDesc.ByteWidth = (UINT)count * sizeof(unsigned short);
	bDesc.StructureByteStride = sizeof(unsigned short);
	bDesc.MiscFlags = 0u;
	bDesc.CPUAccessFlags = 0u;

	D3D11_SUBRESOURCE_DATA sData;
	sData.pSysMem = indices.data();

	GetDevice(gfx)->CreateBuffer(&bDesc, &sData, &pIndexBuffer);
}

void IndexBuffer::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept
{
	return count;
}
