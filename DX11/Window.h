#pragma once
#include "NastihanWin.h"
#include "NastihanException.h"
#include "Keyboard.h"
#include "Mouse.h"

class Window
{
public:
	class WindowException : NastihanException
	{
	public:
		WindowException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};

private:
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "Engine Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
	};

public:
	Window(int width, int height, const char* name) ;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
	Keyboard kbd;
	Mouse mouse;
	
private:
	int width;
	int height;
	HWND hWnd;

};

// error exception helper macro
#define CHWND_EXCEPT( hr ) Window::WindowException( __LINE__,__FILE__,hr )
#define CHWND_LAST_EXCEPT() Window::WindowException( __LINE__,__FILE__,GetLastError() )