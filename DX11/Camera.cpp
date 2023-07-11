#include "Camera.h"
#include "imgui/imgui.h"
#include "NastihanMath.h"

Camera::Camera(std::string name, DirectX::XMFLOAT3 homePos, float homePitch, float homeYaw) noexcept
	:
	name(std::move(name)),
	homePos(homePos),
	homePitch(homePitch),
	homeYaw(homeYaw)
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

void Camera::SpawnControlWidgets() noexcept
{
	ImGui::Text("Position");
	ImGui::SliderFloat("X", &pos.x, -80.0f, 80.0f, "%.1f");
	ImGui::SliderFloat("Y", &pos.y, -80.0f, 80.0f, "%.1f");
	ImGui::SliderFloat("Z", &pos.z, -80.0f, 80.0f, "%.1f");
	ImGui::Text("Orientation");
	ImGui::SliderAngle("Pitch", &pitch, 0.990f * -90.0f, 0.990f * 90.0f);
	ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
	if (ImGui::Button("Reset"))
	{
		Reset();
	}
}

void Camera::Reset() noexcept
{
	pos = homePos;
	pitch = homePitch;
	yaw = homeYaw;
}

const std::string& Camera::GetName() const noexcept
{
	return name;
}