#include "winop.hpp"
#include "dwmapi.h"
#include <sstream>
#include <regex>
#include <QThread>
#include <QDebug>

namespace {
	struct FindChildParam {
		std::wstring    target;
		HWND            hWnd{};

		void setResult(HWND h) {
			hWnd = h;
		}
	};
}

namespace dg {
	RECT GetWindowRectDwm(const HWND hw) {
		RECT rect;
		const auto ret = DwmGetWindowAttribute(hw, DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(rect));
		if(ret == E_HANDLE) {
			// hw = 何かの子ウィンドウ
			GetWindowRect(hw, &rect);
		}
		return rect;
	}
	bool CheckContainsSubstr(const std::wstring& str, const std::wstring& target) {
		std::wstringstream ss;
		ss << LR"(.*(\s*))" << target << LR"(.*)";
		const std::wregex re(ss.str());
		return std::regex_match(str, re);
	}
	bool CheckContainsWindowName(const HWND hw, const std::wstring& target) {
		TCHAR szBuff[MAX_WINDOW_NAME];
		GetWindowText(hw, szBuff, _countof(szBuff));
		return CheckContainsSubstr(szBuff, target);
	}

	BOOL CALLBACK EnumChild(const HWND hw, const LPARAM lParam) {
		auto& param = *reinterpret_cast<FindChildParam*>(lParam);  // NOLINT(performance-no-int-to-ptr)
		if(CheckContainsWindowName(hw, param.target)) {
			param.setResult(hw);
			return FALSE;
		}
		return TRUE;
	}
	void ClickLeftTop(const HWND hw, const bool rightButton, WORD offset) {
		const LPARAM OFS = MAKELPARAM(offset, offset);
		if(rightButton) {
			SendMessage(hw, WM_RBUTTONDOWN, MK_RBUTTON, OFS);
			SendMessage(hw, WM_RBUTTONUP, 0, OFS);
		} else {
			SendMessage(hw, WM_LBUTTONDOWN, MK_LBUTTON, OFS);
			SendMessage(hw, WM_LBUTTONUP, 0, OFS);
		}
	}
	void TapKey(const int vkey, const WORD auxCode) {
		INPUT input[4] = {};
		auto* inp = input;
		if(auxCode != 0)
		{
			inp->type = INPUT_KEYBOARD;
			inp->ki.wVk = auxCode;
			++inp;
		}
		inp->type = INPUT_KEYBOARD;
		inp->ki.wVk = vkey;
		++inp;

		inp->type = INPUT_KEYBOARD;
		inp->ki.wVk = vkey;
		inp->ki.dwFlags = KEYEVENTF_KEYUP;
		++inp;

		if(auxCode != 0)
		{
			inp->type = INPUT_KEYBOARD;
			inp->ki.wVk = auxCode;
			inp->ki.dwFlags = KEYEVENTF_KEYUP;
			++inp;
		}
		SendInput(
			static_cast<UINT>(inp - input),
			input,
			sizeof(INPUT)
		);
	}
	void TempSwitch(const HWND target, const Proc& proc, const uint32_t wait) {
		const auto hwOrig = GetForegroundWindow();
		SetForegroundWindow(target);
		QThread::msleep(wait);
		proc();
		QThread::msleep(wait);
		SetForegroundWindow(hwOrig);
	}

	BOOL CALLBACK WndEnumProc(const HWND hw, const LPARAM lParam) {
		auto* p = reinterpret_cast<FindWindowParam*>(lParam);
		if(CheckContainsWindowName(hw, p->targetTopName)) {
			if(p->targetChildName.empty()) {
				p->result = hw;
				return FALSE;
			} else {
				FindChildParam fp;
				fp.target = p->targetChildName;
				EnumChildWindows(hw, EnumChild, reinterpret_cast<LPARAM>(&fp));
				if((p->result = fp.hWnd))
					return FALSE;
			}
		}
		return TRUE;
	}

	QString GetWindowName(const HWND hWnd) {
		TCHAR name[MAX_WINDOW_NAME];
		GetWindowText(hWnd, name, _countof(name));
		return QString::fromWCharArray(name);
	}
	void PrintAllWindow(const HWND hWnd) {
		if(!hWnd)
			return;
		PrintWindowName(hWnd);

		HWND cur = nullptr;
		do
		{
			cur = FindWindowEx(hWnd, cur, nullptr, nullptr);
			PrintAllWindow(cur);
		} while(cur);
	}
	void PrintWindowName(const HWND hWnd, int indent) {
		constexpr int MAX_INDENT = 64;
		TCHAR indent_buff[MAX_INDENT];
		indent_buff[0] = TEXT('\0');
		if(indent > 0) {
			indent = std::min<int>(MAX_INDENT, indent);

			auto* buffp =  indent_buff;
			for(int i=0 ; i<indent ; i++) {
				*buffp++ = TEXT(' ');
			}
			*buffp = TEXT('\0');
		}
		qDebug() << QString::fromWCharArray(indent_buff) + GetWindowName(hWnd);
	}
}
