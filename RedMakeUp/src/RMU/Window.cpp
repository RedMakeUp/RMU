#include "stdafx.h"
#include "Window.h"

namespace RMU {
	/// <summary>
	/// Initialize window class singleton
	/// </summary>
	Window::WindowClass Window::WindowClass::s_windowClass;


	inline const wchar_t* Window::WindowClass::GetName() noexcept
	{
		return s_className;
	}

	inline HINSTANCE Window::WindowClass::GetInstance() noexcept
	{
		return s_windowClass.m_hInst;
	}

	Window::WindowClass::WindowClass() noexcept 
		:m_hInst(GetModuleHandle(nullptr))
	{
		WNDCLASSEX wc = {};// Zero Initialization
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = DefWindowProc; // TODO: Custom message handler
		wc.hInstance = GetInstance();
		wc.lpszClassName = GetName();
		RegisterClassEx(&wc);
	}

	Window::WindowClass::~WindowClass(){
		UnregisterClass(GetName(), GetInstance());
	}
}