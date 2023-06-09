#pragma once
#include "Drawable.h"
#include "Bindable.h"
#include "IndexBuffer.h"

class TestCube : public Drawable
{
public:
	TestCube(Graphics& gfx, float size);
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	void SetRotation(float roll, float pitch, float yaw) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SpawnControlWindow(Graphics& gfx, const char* name) noexcept;
	void DrawOutline(Graphics& gfx) noxnd
	{
		outlining = true;
		for (auto& b : outlineEffect)
		{
			b->Bind(gfx);
		}
		//gfx.DrawIndexed(QueryBindable<Bind::IndexBuffer>()->GetCount());
		outlining = false;
	}
private:
	std::vector<std::shared_ptr<Bind::Bindable>> outlineEffect;
	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	bool outlining = false;
};