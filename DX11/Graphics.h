#pragma once
#include "NastihanWin.h"
#include "NastihanException.h"
#include <d3d11.h>
#include <vector>
#include <wrl.h>
#include "DxgiInfoManager.h"

class Graphics
{
public:

	class Exception : public NastihanException
	{
		using NastihanException::NastihanException;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;

	private:
		HRESULT hr;
		std::string info;

	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};

	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	void ClearBuffer();
	void EndFrame();

private:
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context ;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> targetView;
};