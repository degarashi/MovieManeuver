#include "manip_vlc.hpp"
#include "winop.hpp"
#include "manip_param.hpp"
#include <QThread>
#include <QDateTime>

namespace dg {
	namespace {
		constexpr unsigned long MANIP_WAIT = 25;
		constexpr unsigned long VOLUME_MANIP_OFFSET = 160;
	}
	void Manip_VLC::setFocus(const HWND hw) const {
        // ウィンドウ下部中央をクリックする
		const RECT r = GetWindowRectDwm(hw);
		ClickLeftTop(hw, MouseButton::Left, false, (r.right-r.left)/2, r.bottom-r.top-12);
        QThread::msleep(MANIP_WAIT);
	}
	const std::wstring& Manip_VLC::getWindowTopName() const {
		static std::wstring ret(L"VLC media player");
		return ret;
	}
	const std::wstring& Manip_VLC::getWindowChildName() const {
		static std::wstring ret;
		return ret;
	}
	void Manip_VLC::startPause(const ManipParam& param) const {
        TapKey(VK_SPACE);
    }
	void Manip_VLC::forward_few(const ManipParam& param) const {
        TapKey(VK_RIGHT, VK_SHIFT);
    }
	void Manip_VLC::backward_few(const ManipParam& param) const {
        TapKey(VK_LEFT, VK_SHIFT);
    }
	void Manip_VLC::forward_medium(const ManipParam& param) const {
        TapKey(VK_RIGHT, VK_MENU);
    }
	void Manip_VLC::backward_medium(const ManipParam& param) const {
        TapKey(VK_LEFT, VK_MENU);
    }
	void Manip_VLC::speedDown(const ManipParam& param) const {
		// 音のピッチを下げてスピードダウン
		// TapKey(VK_OEM_4);	// [
		// 音のピッチを下げずにスピードダウン
		TapKey(VK_SUBTRACT);
    }
	void Manip_VLC::speedUp(const ManipParam& param) const {
		// 音のピッチを上げてスピードアップ
		// TapKey(VK_OEM_6);	// ]
		// 音のピッチを上げずにスピードアップ
		TapKey(VK_ADD);
    }
	void Manip_VLC::volumeDown(const ManipParam& param) const {
        TapKey(VK_DOWN, VK_CONTROL);
    }
	void Manip_VLC::volumeUp(const ManipParam& param) const {
        TapKey(VK_UP, VK_CONTROL);
    }
	void Manip_VLC::volumeMute(const ManipParam& param) const {
        TapKey(u8'M');
    }

	void Manip_VLC::fullScreen(const ManipParam& param) const {
        TapKey(u8'F');
    }
	void Manip_VLC::captionSwitch(const ManipParam& param) const {
        TapKey(u8'V');
    }
	const QString& Manip_VLC::getName() const {
		static QString Name = QString::fromUtf8(u8"VLC Manipulator");
		return Name;
	}
}
