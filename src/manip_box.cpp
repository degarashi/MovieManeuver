#include "manip_box.hpp"
#include "winop.hpp"
#include <QThread>

namespace dg {
    void Manip_Box::setFocus(HWND hw) const {
        ClickLeftTop(hw, false, false, 192);
        QThread::msleep(20);
	}
	void Manip_Box::startPause(const HWND hw) const {
        TapKey(VK_SPACE);
    }
	void Manip_Box::forward_few(const HWND hw) const {
        TapKey(VK_RIGHT);
    }
	void Manip_Box::backward_few(const HWND hw) const {
        TapKey(VK_LEFT);
    }
	void Manip_Box::forward_medium(const HWND hw) const {
        TapKey(u8'L');
    }
	void Manip_Box::backward_medium(const HWND hw) const {
        TapKey(u8'J');
    }
	void Manip_Box::speedDown(const HWND hw) const {
        TapKey(0xBC, VK_SHIFT);
    }
	void Manip_Box::speedUp(const HWND hw) const {
        TapKey(0xBE, VK_SHIFT);
    }
	void Manip_Box::volumeDown(const HWND hw) const {
        TapKey(VK_DOWN);
    }
	void Manip_Box::volumeUp(const HWND hw) const {
        TapKey(VK_UP);
    }
	void Manip_Box::volumeMute(const HWND hw) const {
        TapKey(u8'M');
    }
	void Manip_Box::captionSwitch(const HWND hw) const {
        TapKey(u8'C');
    }

	const std::wstring& Manip_Box::getWindowTopName() const {
		static std::wstring ret(L"Powered by Box");
		return ret;
	}
	const std::wstring& Manip_Box::getWindowChildName() const {
		static std::wstring ret(L"Legacy Window");
		return ret;
	}
	const QString& Manip_Box::getName() const {
		static QString Name = QString::fromUtf8(u8"Box Manipulator");
		return Name;
	}
}
