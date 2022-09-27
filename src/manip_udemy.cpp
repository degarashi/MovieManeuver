#include "manip_udemy.hpp"
#include "winop.hpp"
#include <QThread>
#include <QDateTime>

namespace dg {
	namespace {
		constexpr unsigned long WAIT = 200;
		constexpr u_int64 FOCUS_THRESHOLD_MS = 1000;
	}
	void Manip_Udemy::_focus(HWND hw, const Proc_t& p) const {
		const u_int64 cur = QDateTime::currentMSecsSinceEpoch();
		// 短時間の間に連続で何か処理をする場合はフォーカスの処理をしない
		if((cur - _prevTime) >= FOCUS_THRESHOLD_MS) {
			TempSwitch(hw, [hw, &p](){
				ClickLeftTop(hw, true, 192);
				QThread::msleep(20);
				TapKey(VK_HOME);
				p();
			});
		} else {
			TempSwitch(hw, [hw, &p](){
				p();
			});
		}
		_prevTime = cur;
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
		_focus(hw, [](){
			TapKey(VK_SPACE);
		});
	}
	void Manip_Udemy::forward_5sec(const HWND hw) const {
		_focus(hw, [](){
			TapKey(VK_RIGHT);
		});
	}
	void Manip_Udemy::backward_5sec(const HWND hw) const {
		_focus(hw, [](){
			TapKey(VK_LEFT);
		});
	}
	void Manip_Udemy::forward_10sec(const HWND hw) const {
		_focus(hw, [](){
			TapKey(VK_RIGHT);
			QThread::msleep(WAIT);
			TapKey(VK_RIGHT);
		});
	}
	void Manip_Udemy::backward_10sec(const HWND hw) const {
		_focus(hw, [](){
			TapKey(VK_LEFT);
			QThread::msleep(WAIT);
			TapKey(VK_LEFT);
		});
	}
	void Manip_Udemy::speedDown(const HWND hw) const {
		_focus(hw, [](){
			TapKey(VK_LEFT, VK_SHIFT);
		});
	}
	void Manip_Udemy::speedUp(const HWND hw) const {
		_focus(hw, [](){
			TapKey(VK_RIGHT, VK_SHIFT);
		});
	}
	void Manip_Udemy::volumeDown(const HWND hw) const {
		_focus(hw, [](){
			TapKey(VK_DOWN);
		});
	}
	void Manip_Udemy::volumeUp(const HWND hw) const {
		_focus(hw, [](){
			TapKey(VK_UP);
		});
	}
	void Manip_Udemy::volumeMute(const HWND hw) const {
		_focus(hw, [](){
			TapKey(u8'M');
		});
	}
	void Manip_Udemy::fullScreen(const HWND hw) const {
		_focus(hw, [](){
			TapKey(u8'F');
		});
	}
	const QString& Manip_Udemy::getName() const {
		static QString Name = QString::fromUtf8(u8"Udemy Manipulator");
		return Name;
	}
}
