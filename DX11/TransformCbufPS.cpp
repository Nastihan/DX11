#include "TransformCbufPS.h"

//namespace Bind
//{
//	TransformCbufPS::TransformCbufPS(Graphics& gfx, const Drawable& parent, UINT slotV, UINT slotP)
//		:
//		TransformCbuf(gfx, parent, slotV)
//	{
//		if (!pPcbuf)
//		{
//			pPcbuf = std::make_unique<PixelConstantBuffer<Transforms>>(gfx, slotP);
//		}
//	}
//
//	void Bind::TransformCbufPS::Bind(Graphics& gfx) noexcept
//	{
//		const auto tf = GetTransforms(gfx);
//		TransformCbuf::UpdateBindImpl(gfx, tf);
//		UpdateBindImpl(gfx, tf);
//	}
//
//	void TransformCbufPS::UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept
//	{
//		pPcbuf->Update(gfx, tf);
//		pPcbuf->Bind(gfx);
//	}
//
//	std::unique_ptr<PixelConstantBuffer<TransformCbuf::Transforms>> TransformCbufPS::pPcbuf;
//}