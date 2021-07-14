#include "stdafx.h"
#include "Window.h"

namespace RMU {
	const wchar_t* Window::REGISTERED_NAME = L"RedMakeUp Engine";

	bool Window::Register()
	{
		WNDCLASSEX wc = {};// Zero Initialization
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = HandleMsgSetup;
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpszClassName = REGISTERED_NAME;
		static ATOM atom = RegisterClassEx(&wc);

		if (atom > 0) {
			RMU_LOG_INFO("Registered window class");
			return true;
		}
		else {
			RMU_LOG_ERROR("Failed to register window class");
			return false;
		}
	}

	Window::Window(int width, int height, const wchar_t* name)
		:m_height(height), m_width(width)
	{
		// Custom window style
		auto style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

		// Calculate the whole window size based on given client size
		RECT rect;
		rect.left = 100;
		rect.right = rect.left + width;
		rect.top = 100;
		rect.bottom = rect.top + height;
		AdjustWindowRect(&rect, style, FALSE);

		// Create the actual window
		m_hWnd = CreateWindow(
			REGISTERED_NAME,
			name,
			style,
			CW_USEDEFAULT, CW_USEDEFAULT,
			rect.right - rect.left,
			rect.bottom - rect.top,
			nullptr, nullptr,
			GetModuleHandle(nullptr),
			this
		);

		// Show this window
		ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	}

	Window::~Window()
	{
		
	}

	std::optional<int> Window::ProcessMessages()
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// Check for quit because peekmessage does not signal this via return val
			if (msg.message == WM_QUIT)
			{
				// Return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
				return (int)msg.wParam;
			}

			// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Return empty optional when not quitting app
		return {};
	}

	LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
		if (msg == WM_NCCREATE)
		{
			// extract ptr to window class from creation data
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
			// set WinAPI-managed user data to store ptr to window class
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			// set message proc to normal (non-setup) handler now that setup is finished
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
			// forward message to window class handler
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}
		// if we get a message before the WM_NCCREATE message, handle with default handler
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// retrieve ptr to window class
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		switch (msg)
		{
		case WM_CLOSE:
			{
				RMU_LOG_WARN("Attemp to close main window!");
				int code = MessageBoxA(NULL, "Really Exit?", "Exit", MB_YESNO);
				if (code == IDYES) {
					DestroyWindow(m_hWnd);
					RMU_LOG_WARN("Main window will be closed");
				}
				else {
					RMU_LOG_WARN("Cancle to close main window");
				}
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return 0;
	}
}