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
		//ImGui::SliderFloat("R", &r, 0.0f, 80.0f, "%.1f");
		//ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
		//ImGui::SliderAngle("Phi", &phi, -89.0f, 89.0f);
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
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
	pos = { 0.0f, 7.0f, -25.0f };
	pitch = 0.0f;
	yaw = 0.0f;
}
