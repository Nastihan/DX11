#include "Blender.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"


namespace Bind
{

    Bind::Blender::Blender(Graphics& gfx, bool blending)

    {
        INFOMAN(gfx);

        D3D11_BLEND_DESC bDesc{};
        auto& brt = bDesc.RenderTarget[0];
        
        if (blending)
        {
            brt.BlendEnable = true;
            brt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
            brt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            brt.BlendOp = D3D11_BLEND_OP_ADD;
            brt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
            brt.SrcBlendAlpha = D3D11_BLEND_ZERO;
            brt.DestBlendAlpha = D3D11_BLEND_ZERO;
            brt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        }
        else
        {
            brt.BlendEnable = false;
            brt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        }

        GFX_THROW_INFO_ONLY(GetDevice(gfx)->CreateBlendState(&bDesc, pBlender.GetAddressOf()));

    }

    void Bind::Blender::Bind(Graphics& gfx) noexcept
    {
        GetContext(gfx)->OMSetBlendState(pBlender.Get(), nullptr, 0xFFFFFFFF);
    }

    std::shared_ptr<Blender> Bind::Blender::Resolve(Graphics& gfx, bool blending)
    {
        return Codex::Resolve<Blender>(gfx,blending);
    }

    std::string Bind::Blender::GenerateUID(bool blending)
    {
        using namespace std::string_literals;
        return typeid(Blender).name() + "#"s + (blending ? "b" : "n");
    }

    std::string Bind::Blender::GetUID() const noexcept
    {
        return GenerateUID(blending);
    }

}