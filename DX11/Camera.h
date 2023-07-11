#pragma once
#include "Graphics.h"

class Camera
{
	public:
		Camera(DirectX::XMFLOAT3 homePos = { 0.0f,0.0f,0.0f }, float homePitch = 0.0f, float homeYaw = 0.0f, std::string name = "cam") noexcept;
		DirectX::XMMATRIX GetMatrix() const noexcept;
		void Rotate(float dx, float dy) noexcept;
		void Translate(DirectX::XMFLOAT3 translation) noexcept;
		std::string GetName()
		{
			return name;
		}
		void SpawnControlWindow() noexcept;
		void Reset() noexcept;
	private:
		std::string name;
		DirectX::XMFLOAT3 homePos;
		float homePitch;
		float homeYaw;
		DirectX::XMFLOAT3 pos;
		float pitch;
		float yaw;
		static constexpr float travelSpeed = 12.0f;
		static constexpr float rotationSpeed = 0.004f;
	
};