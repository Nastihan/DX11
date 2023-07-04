#include "Camera.h"
#include "imgui/imgui.h"
#include "NastihanMath.h"

Camera::Camera()
{
	Reset();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	
	DirectX::FXMVECTOR eye{DirectX::FXMVECTOR{pos.x, pos.y, pos.z}};
	auto rotation = DirectX::XMMatrixRotationRollPitchYaw (pitch, yaw, 0.0f);
	DirectX::FXMVECTOR eyeDir{0.0f, 0.0f, 1.0f};
	auto camDir = DirectX::XMVector3Transform(eyeDir, rotation);
	DirectX::FXMVECTOR upDir{0.0f, 1.0f, 0.0f};

	return DirectX::XMMatrixLookToLH(eye, camDir, upDir);
}

void Camera::Rotate(float dx, float dy) noexcept
{
	yaw = wrap_angle(yaw + dx * rotationSpeed);
	pitch = std::clamp(pitch + dy * rotationSpeed, 0.995f * -PI / 2.0f, 0.995f * PI / 2.0f);
}

void Camera::Translate(DirectX::XMFLOAT3 translation) noexcept
{
	
	auto t = DirectX::XMLoadFloat3(&translation);
	t = DirectX::XMVector3Transform(t, DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
		DirectX::XMMatrixScaling(travelSpeed, travelSpeed, travelSpeed));
	
	pos.x += DirectX::XMVectorGetX(t);
	pos.y += DirectX::XMVectorGetY(t);
	pos.z += DirectX::XMVectorGetZ(t);
}

void Camera::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("x", &pos.x, -100.0f, 100.0f, "%.1f");
		ImGui::SliderFloat("y", &pos.y, -100.0f, 100.0f);
		ImGui::SliderFloat("z", &pos.z, -100.0f, 100.0f);
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Pitch", &pitch, -89.0f, 89.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void Camera::Reset() noexcept
{
	pos = { 0.0f, 2.0f, -15.0f };
	pitch = 0.0f;
	yaw = 0.0f;//PI / 2.0f;
}
