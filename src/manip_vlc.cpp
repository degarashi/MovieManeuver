#include "manip_vlc.hpp"
#include "winop.hpp"
#include <QThread>
#include <QDateTime>

namespace dg {
	namespace {
		constexpr unsigned long MANIP_WAIT = 25;
		constexpr unsigned long VOLUME_MANIP_OFFSET = 160;
	}
	void Manip_VLC::_Focus(HWND hw, const Proc_t& p) {
		TempSwitch(hw, [hw, &p](){
			// ウィンドウ下部中央をクリックする
			const RECT r = GetWindowRectDwm(hw);
			ClickLeftTop(hw, false, false, (r.right-r.left)/2, r.bottom-r.top-2);
			QThread::msleep(MANIP_WAIT);
			p();
		});
	}
	const std::wstring& Manip_VLC::getWindowTopName() const {
		static std::wstring ret(L"VLC media player");
		return ret;
	}
	const std::wstring& Manip_VLC::getWindowChildName() const {
		static std::wstring ret;
		return ret;
	}
	void Manip_VLC::startPause(const HWND hw) const {
		_Focus(hw, [](){
			TapKey(VK_SPACE);
		});
	}
	void Manip_VLC::forward_few(const HWND hw) const {
		_Focus(hw, [](){
			TapKey(VK_RIGHT, VK_SHIFT);
		});
	}
	void Manip_VLC::backward_few(const HWND hw) const {
		_Focus(hw, [](){
			TapKey(VK_LEFT, VK_SHIFT);
		});
	}
	void Manip_VLC::forward_medium(const HWND hw) const {
		_Focus(hw, [](){
			TapKey(VK_RIGHT, VK_MENU);
		});
	}
	void Manip_VLC::backward_medium(const HWND hw) const {
		_Focus(hw, [](){
			TapKey(VK_LEFT, VK_MENU);
		});
	}
	void Manip_VLC::speedDown(const HWND hw) const {
		_Focus(hw, [](){
			TapKey(VK_OEM_4);	// [
		});
	}
	void Manip_VLC::speedUp(const HWND hw) const {
		_Focus(hw, [](){
			TapKey(VK_OEM_6);	// ]
		});
	}
	void Manip_VLC::volumeDown(const HWND hw) const {
		_Focus(hw, [](){
			TapKey(VK_DOWN, VK_CONTROL);
		});
	}
	void Manip_VLC::volumeUp(const HWND hw) const {
		_Focus(hw, [](){
			TapKey(VK_UP, VK_CONTROL);
		});
	}
	void Manip_VLC::volumeMute(const HWND hw) const {
		_Focus(hw, [](){
			TapKey(u8'M');
		});
	}

	void Manip_VLC::fullScreen(HWND hw) const {
		_Focus(hw, [](){
			TapKey(u8'F');
		});
	}
	void Manip_VLC::captionSwitch(const HWND hw) const {
		_Focus(hw, [](){
			TapKey(u8'V');
		});
	}
	const QString& Manip_VLC::getName() const {
		static QString Name = QString::fromUtf8(u8"VLC Manipulator");
		return Name;
	}
}
