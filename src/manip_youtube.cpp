#include "manip_youtube.hpp"
#include "winop.hpp"
#include "manip_param.hpp"
#include <QThread>
#include <QDateTime>

namespace dg {
	namespace {
		constexpr unsigned long MANIP_WAIT = 25;
		constexpr unsigned long MANIP_OFFSET = 160;
	}
	void Manip_YouTube::setFocus(const HWND hw) const {
		TapKey(VK_IME_OFF);
		if(!IsFullScreen(hw)) {
            // ページの先頭に戻る
			ClickLeftTop(hw, MouseButton::Right, false, 2);
            QThread::msleep(MANIP_WAIT);
            TapKey(VK_ESCAPE);
            QThread::msleep(MANIP_WAIT);
            TapKey(VK_HOME);
        } else {
			ClickLeftTop(hw, MouseButton::Right, true, MANIP_OFFSET);
            QThread::msleep(MANIP_WAIT);
			ClickLeftTop(hw, MouseButton::Left, true, MANIP_OFFSET-2);
        }
	}
	const std::wstring& Manip_YouTube::getWindowTopName() const {
		static std::wstring ret(L"YouTube");
		return ret;
	}
	const std::wstring& Manip_YouTube::getWindowChildName() const {
		static std::wstring ret(L"Legacy Window");
		return ret;
	}
	void Manip_YouTube::startPause(const ManipParam& param) const {
        TapKey(u8'K');
    }
	void Manip_YouTube::forward_few(const ManipParam& param) const {
        TapKey(VK_RIGHT);
    }
	void Manip_YouTube::backward_few(const ManipParam& param) const {
        TapKey(VK_LEFT);
    }
	void Manip_YouTube::forward_medium(const ManipParam& param) const {
        TapKey(u8'L');
    }
	void Manip_YouTube::backward_medium(const ManipParam& param) const {
        TapKey(u8'J');
    }
	void Manip_YouTube::speedDown(const ManipParam& param) const {
        TapKey(0xBC, VK_SHIFT);
    }
	void Manip_YouTube::speedUp(const ManipParam& param) const {
        TapKey(0xBE, VK_SHIFT);
    }
	void Manip_YouTube::volumeDown(const ManipParam& param) const {
		if(!IsFullScreen(param.hw)) {
            QThread::msleep(MANIP_WAIT);
			ClickLeftTop(param.hw, MouseButton::Right, false, MANIP_OFFSET);
            TapKey(VK_ESCAPE);
        }
        TapKey(VK_DOWN);
    }
	void Manip_YouTube::volumeUp(const ManipParam& param) const {
		if(!IsFullScreen(param.hw)) {
            QThread::msleep(MANIP_WAIT);
			ClickLeftTop(param.hw, MouseButton::Right, false, MANIP_OFFSET);
            TapKey(VK_ESCAPE);
        }
        TapKey(VK_UP);
    }
	void Manip_YouTube::volumeMute(const ManipParam& param) const {
        TapKey(u8'M');
    }

	void Manip_YouTube::fullScreen(const ManipParam& param) const {
        TapKey(u8'F');
    }
	void Manip_YouTube::captionSwitch(const ManipParam& param) const {
        TapKey(u8'C');
    }
	void Manip_YouTube::seekByNumber(const ManipParam& param) const {
		const auto vk = u8'0' + param.aux0;
		TapKey(vk);
	}
	const QString& Manip_YouTube::getName() const {
		static QString Name = QString::fromUtf8(u8"Youtube Manipulator");
		return Name;
	}
}
