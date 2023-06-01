#include "PointLight.h"
#include "imgui/imgui.h"

PointLight::PointLight(Graphics& gfx, float radius)
	:
	mesh(gfx, radius),
	cbuf(gfx,0u)
{}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &cBufData.pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &cBufData.pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &cBufData.pos.z, -60.0f, 60.0f, "%.1f");
		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Diffuse Intensity", &cBufData.diffuseIntensity, 0.0f, 1.0f, "%.1f");
		ImGui::ColorEdit3("Diffuse Color", &cBufData.diffuseColor.x);
		ImGui::ColorEdit3("Ambient Color", &cBufData.ambient.x);
		ImGui::Text("Attenuation");
		ImGui::SliderFloat("Constant", &cBufData.attConst, 0.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("Linear", &cBufData.attLin, 0.0f, 10.0f, "%.3f",4);
		ImGui::SliderFloat("Quadratic", &cBufData.attQuad, 0.0f, 10.0f, "%.4f",10);
		if (ImGui::Button("Reset")) 
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	cBufData.pos = { 0.0f,0.0f,0.0f };
	cBufData.ambient = { 0.01f,0.01f,0.01f };
	cBufData.diffuseColor = { 1.0f,1.0f,1.0f };
	cBufData.diffuseIntensity = 1.0f;
	cBufData.attConst = 0.5f;
	cBufData.attLin = 0.045f;
	cBufData.attQuad = 0.0075f;
}

void PointLight::Draw(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	mesh.SetPos(cBufData.pos);
	mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx) const noexcept
{
	cbuf.Update(gfx, PointLightCBuf{ cBufData });
	cbuf.Bind(gfx);
}
