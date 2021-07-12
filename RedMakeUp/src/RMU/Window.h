#pragma once

namespace RMU {
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
	};
}