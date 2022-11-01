#include "manip_youtube.hpp"
#include "winop.hpp"
#include <QThread>
#include <QDateTime>

namespace dg {
	namespace {
		constexpr unsigned long MANIP_WAIT = 25;
		constexpr unsigned long MANIP_OFFSET = 160;
	}
	void Manip_YouTube::_focus(HWND hw, const Proc_t& p) const {
		TempSwitch(hw, [hw, &p](){
			if(!IsFullScreen(hw)) {
				// ページの先頭に戻る
				ClickLeftTop(hw, true);
				QThread::msleep(MANIP_WAIT);
				TapKey(VK_ESCAPE);
				QThread::msleep(MANIP_WAIT);
				TapKey(VK_HOME);
			} else {
				ClickLeftTop(hw, true, MANIP_OFFSET);
				QThread::msleep(MANIP_WAIT);
				ClickLeftTop(hw, false, MANIP_OFFSET);
			}
			p();
		});
	}
	const std::wstring& Manip_YouTube::getWindowTopName() const {
		static std::wstring ret(L"YouTube");
		return ret;
	}
	const std::wstring& Manip_YouTube::getWindowChildName() const {
		static std::wstring ret(L"Legacy Window");
		return ret;
	}
	void Manip_YouTube::startPause(const HWND hw) const {
		_focus(hw, [](){
			TapKey(u8'K');
		});
	}
	void Manip_YouTube::forward_few(const HWND hw) const {
		_focus(hw, [](){
			TapKey(VK_RIGHT);
		});
	}
	void Manip_YouTube::backward_few(const HWND hw) const {
		_focus(hw, [](){
			TapKey(VK_LEFT);
		});
	}
	void Manip_YouTube::forward_medium(const HWND hw) const {
		_focus(hw, [](){
			TapKey(u8'L');
		});
	}
	void Manip_YouTube::backward_medium(const HWND hw) const {
		_focus(hw, [](){
			TapKey(u8'J');
		});
	}
	void Manip_YouTube::speedDown(const HWND hw) const {
		_focus(hw, [](){
			TapKey(0xBC, VK_SHIFT);
		});
	}
	void Manip_YouTube::speedUp(const HWND hw) const {
		_focus(hw, [](){
			TapKey(0xBE, VK_SHIFT);
		});
	}
	void Manip_YouTube::volumeDown(const HWND hw) const {
		_focus(hw, [hw](){
			if(!IsFullScreen(hw)) {
				QThread::msleep(MANIP_WAIT);
				ClickLeftTop(hw, true, MANIP_OFFSET);
				TapKey(VK_ESCAPE);
			}
			TapKey(VK_DOWN);
		});
	}
	void Manip_YouTube::volumeUp(const HWND hw) const {
		_focus(hw, [hw](){
			if(!IsFullScreen(hw)) {
				QThread::msleep(MANIP_WAIT);
				ClickLeftTop(hw, true, MANIP_OFFSET);
				TapKey(VK_ESCAPE);
			}
			TapKey(VK_UP);
		});
	}
	void Manip_YouTube::volumeMute(const HWND hw) const {
		_focus(hw, [](){
			TapKey(u8'M');
		});
	}

	void Manip_YouTube::fullScreen(HWND hw) const {
		_focus(hw, [](){
			TapKey(u8'F');
		});
	}
	void Manip_YouTube::captionSwitch(const HWND hw) const {
		_focus(hw, [](){
			TapKey(u8'C');
		});
	}
	const QString& Manip_YouTube::getName() const {
		static QString Name = QString::fromUtf8(u8"Youtube Manipulator");
		return Name;
	}
}
