#pragma once
#include <DirectXMath.h>
#include "ImGui/imgui.h"

class Projection
{
public:
	Projection(float viewWidth, float viewHeight, float nearZ, float farZ)
		: viewWidth(viewWidth), viewHeight(viewHeight), nearZ(nearZ), farZ(farZ)
	{
	}
	DirectX::XMMATRIX GetMatrix()
	{
		return DirectX::XMMatrixPerspectiveLH(viewWidth, viewHeight, nearZ, farZ);
	}
	void RenderWidgets() noexcept
	{
		ImGui::Text("PRojection Matrix");
		ImGui::SliderFloat("viewWidth", &viewWidth, 0.1, 20);
		ImGui::SliderFloat("viewHieght", &viewHeight, 0.1, 20);
		ImGui::SliderFloat("nearZ", &nearZ, 0.1, 10);
		ImGui::SliderFloat("farZ", &farZ, 0.1, 500);
		/*if (ImGui::Button("Reset"))
		{
			Reset();
		}*/
	}
	void Reset()
	{
		viewWidth = 1.0f;
		viewHeight = 9.0f / 16.0f;
		nearZ = 0.5f;
		farZ = 400.0f;
	}
private:
	float viewWidth;
	float viewHeight;
	float nearZ;
	float farZ;
};