#include "manip_youtube.hpp"
#include "winop.hpp"
#include <QThread>

namespace dg {
	namespace {
		constexpr unsigned long WAIT = 25;
	}
	void Manip_YouTube::YProc(HWND hw, const Proc_t& p) {
		TempSwitch(hw, [hw, &p](){
			ClickLeftTop(hw);
			QThread::msleep(WAIT);
			TapKey(VK_HOME);
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
		YProc(hw, [](){
			TapKey(u8'K');
		});
	}
	void Manip_YouTube::forward_5sec(const HWND hw) const {
		YProc(hw, [](){
			TapKey(VK_RIGHT);
		});
	}
	void Manip_YouTube::backward_5sec(const HWND hw) const {
		YProc(hw, [](){
			TapKey(VK_LEFT);
		});
	}
	void Manip_YouTube::forward_10sec(const HWND hw) const {
		YProc(hw, [](){
			TapKey(u8'L');
		});
	}
	void Manip_YouTube::backward_10sec(const HWND hw) const {
		YProc(hw, [](){
			TapKey(u8'J');
		});
	}
	void Manip_YouTube::speedDown(const HWND hw) const {
		YProc(hw, [](){
			TapKey(0xBC, VK_SHIFT);
		});
	}
	void Manip_YouTube::speedUp(const HWND hw) const {
		YProc(hw, [](){
			TapKey(0xBE, VK_SHIFT);
		});
	}
	void Manip_YouTube::volumeDown(const HWND hw) const {
		YProc(hw, [hw](){
			QThread::msleep(WAIT);
			ClickLeftTop(hw, true, 160);
			TapKey(VK_DOWN);
			QThread::msleep(WAIT);
			TapKey(VK_ESCAPE);
		});
	}
	void Manip_YouTube::volumeUp(const HWND hw) const {
		YProc(hw, [hw](){
			QThread::msleep(WAIT);
			ClickLeftTop(hw, true, 160);
			TapKey(VK_UP);
			QThread::msleep(WAIT);
			TapKey(VK_ESCAPE);
		});
	}
	void Manip_YouTube::volumeMute(const HWND hw) const {
		YProc(hw, [](){
			TapKey(u8'M');
		});
	}
	const QString& Manip_YouTube::getName() const {
		static QString Name = QString::fromUtf8(u8"Youtube Manipulator");
		return Name;
	}
}