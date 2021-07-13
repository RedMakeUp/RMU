#pragma once

#include <optional>

namespace RMU {
	/// <summary>
	/// Main window
	/// </summary>
	class Window {
	private:
		/// <summary>
		/// Singleton for Window Class
		/// </summary>
		class WindowClass {
		public:
			static const wchar_t* GetName() noexcept;
			static HINSTANCE GetInstance() noexcept;
		private:
			WindowClass() noexcept;
			~WindowClass();

			WindowClass(const WindowClass&) = delete;
			WindowClass& operator=(const WindowClass&) = delete;

			static constexpr const wchar_t* s_className = L"RedMakeUp Engine Main Window";
			static WindowClass s_windowClass;
			HINSTANCE m_hInst = nullptr;
		};
	
	public:
		/// <summary>
		/// Create a main window
		/// </summary>
		/// <param name="width">Width of client area</param>
		/// <param name="height">Height of client area</param>
		/// <param name="name">Title os the window</param>
		Window(int width, int height, const wchar_t* name);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		/// <summary>
		/// Message loop
		/// </summary>
		/// <returns>An integer if the application will be terminated; Otherwise, return an empty entity.</returns>
		static std::optional<int> ProcessMessages();

	private:
		/// <summary>
		/// Message handler set as window class is registered.
		/// This will forward messages from static message handler to a member function.
		/// https://github.com/planetchili/hw3d/blob/T6-End/hw3d/Window.cpp
		/// </summary>
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		/// <summary>
		/// Static wrapper of actual message handler which is a member function.
		/// </summary>
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		/// <summary>
		/// Actual message handler
		/// </summary>
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	private:
		int m_height = 0;
		int m_width = 0;
		HWND m_hWnd = nullptr;
	};
}