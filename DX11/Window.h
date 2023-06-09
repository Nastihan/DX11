#pragma once
#include "NastihanWin.h"
#include "NastihanException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <sstream>
#include <optional>
#include <memory>
#include "WindowsThrowMacros.h"


class Window
{
public:
	class Exception : public NastihanException
	{
		using NastihanException::NastihanException;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};

private:
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass() noexcept;
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "Engine Window";
		static WindowClass wndClass;
		HINSTANCE hInst;

	};

public:
	Window(int width, int height, const char* name) ;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);
	void EnableCursor();
	void DisableCursor();
	static std::optional<int> ProcessMessages() noexcept;
	Graphics& Gfx();
private:
	void HideCursor();
	void ShowCursor();
	void EnableImGuiMouse();
	void DisableImGuiMouse();
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
	Keyboard kbd;
	Mouse mouse;
	
private:
	bool cursorEnabled = false;
	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;

};

