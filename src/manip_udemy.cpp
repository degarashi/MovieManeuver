#include "manip_udemy.hpp"
#include "winop.hpp"
#include <QThread>

namespace dg {
	namespace {
		constexpr unsigned long WAIT = 200;
	}
	void Manip_Udemy::UProc(HWND hw, const Proc_t& p) {
		TempSwitch(hw, [hw, &p](){
			ClickLeftTop(hw, true, 192);
			QThread::msleep(20);
			TapKey(VK_HOME);
			p();
		});
	}
	const std::wstring& Manip_Udemy::getWindowTopName() const {
		static std::wstring ret(L"Udemy");
		return ret;
	}
	const std::wstring& Manip_Udemy::getWindowChildName() const {
		static std::wstring ret(L"Legacy Window");
		return ret;
	}
	void Manip_Udemy::startPause(const HWND hw) const {
		UProc(hw, [](){
			TapKey(VK_SPACE);
		});
	}
	void Manip_Udemy::forward_few(const HWND hw) const {
		UProc(hw, [](){
			TapKey(VK_RIGHT);
		});
	}
	void Manip_Udemy::backward_few(const HWND hw) const {
		UProc(hw, [](){
			TapKey(VK_LEFT);
		});
	}
	void Manip_Udemy::forward_medium(const HWND hw) const {
		UProc(hw, [](){
			TapKey(VK_RIGHT);
			QThread::msleep(WAIT);
			TapKey(VK_RIGHT);
		});
	}
	void Manip_Udemy::backward_medium(const HWND hw) const {
		UProc(hw, [](){
			TapKey(VK_LEFT);
			QThread::msleep(WAIT);
			TapKey(VK_LEFT);
		});
	}
	void Manip_Udemy::speedDown(const HWND hw) const {
		UProc(hw, [](){
			TapKey(VK_LEFT, VK_SHIFT);
		});
	}
	void Manip_Udemy::speedUp(const HWND hw) const {
		UProc(hw, [](){
			TapKey(VK_RIGHT, VK_SHIFT);
		});
	}
	void Manip_Udemy::volumeDown(const HWND hw) const {
		UProc(hw, [](){
			TapKey(VK_DOWN);
		});
	}
	void Manip_Udemy::volumeUp(const HWND hw) const {
		UProc(hw, [](){
			TapKey(VK_UP);
		});
	}
	void Manip_Udemy::volumeMute(const HWND hw) const {
		UProc(hw, [](){
			TapKey(u8'M');
		});
	}
	void Manip_Udemy::fullScreen(const HWND hw) const {
		UProc(hw, [](){
			TapKey(u8'F');
		});
	}
	void Manip_Udemy::captionSwitch(const HWND hw) const {
		UProc(hw, [](){
			TapKey(u8'C');
		});
	}
	const QString& Manip_Udemy::getName() const {
		static QString Name = QString::fromUtf8(u8"Udemy Manipulator");
		return Name;
	}
}
