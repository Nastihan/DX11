#include "Mesh.h"
#include "imgui/imgui.h"
#include "Surface.h"
#include <unordered_map>
#include <sstream>
#include <filesystem>
#include "NastihanXM.h"
#include "DynamicConstant.h"
#include "ConstantBuffersEx.h"
#include "LayoutCodex.h"
#include "Stencil.h"

namespace dx = DirectX;


// Mesh
Mesh::Mesh(Graphics& gfx, const Material& mat, const aiMesh& mesh) noxnd
	:
Drawable(gfx, mat, mesh)
{}

void Mesh::Submit(FrameCommander& frame, dx::FXMMATRIX accumulatedTranform) const noxnd
{
	dx::XMStoreFloat4x4(&transform, accumulatedTranform);
	Drawable::Submit(frame);
}

DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat4x4(&transform);
}