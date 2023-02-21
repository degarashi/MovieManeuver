#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <functional>
#include <string>
#include <QString>

namespace dg {
	bool IsMouseSwapped();
	bool IsFullScreen(HWND hw);
	RECT GetWindowRectDwm(HWND hw);
	constexpr int MAX_WINDOW_NAME = 512;
	bool CheckContainsSubstr(const std::wstring& str, const std::wstring& target);
	bool CheckContainsWindowName(HWND hw, const std::wstring& target);
    UINT GetDoubleClickTime();

	BOOL CALLBACK EnumChild(HWND hw, LPARAM lParam);
	void ClickLeftTop(HWND hw, bool rightButton, bool bUseSendMessage,
					  WORD offsetX, WORD offsetY = std::numeric_limits<WORD>::max());
	void TapKey(int vkey, WORD auxCode = 0);

	using Proc = std::function<void ()>;
	void TempSwitch(HWND target, const Proc& proc, uint32_t wait=1);

	struct FindWindowParam {
		std::wstring    targetTopName,
						targetChildName;
		HWND result;
	};
	BOOL CALLBACK WndEnumProc(HWND hw, LPARAM lParam);

	QString GetWindowName(HWND hWnd);
	void PrintAllWindow(HWND hWnd);
	void PrintWindowName(HWND hWnd, int indent = 0);
}
