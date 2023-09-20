#include "manip_udemy.hpp"
#include "winop.hpp"
#include "manip_param.hpp"
#include <QThread>

namespace dg {
	namespace {
		// キーを連続で押す際のウェイト
		constexpr unsigned long REPEAT_WAIT = 50;
	}
	void Manip_Udemy::setFocus(const HWND hw) const {
		ClickLeftTop(hw, true, true, 192);
		QThread::msleep(20);
		TapKey(VK_IME_OFF);
	}
	const std::wstring& Manip_Udemy::getWindowTopName() const {
		static std::wstring ret(L"Udemy");
		return ret;
	}
	const std::wstring& Manip_Udemy::getWindowChildName() const {
		static std::wstring ret(L"Legacy Window");
		return ret;
	}
	void Manip_Udemy::startPause(const ManipParam& param) const {
		TapKey(VK_SPACE);
	}
	void Manip_Udemy::forward_few(const ManipParam& param) const {
		TapKey(VK_RIGHT);
	}
	void Manip_Udemy::backward_few(const ManipParam& param) const {
		TapKey(VK_LEFT);
	}
	void Manip_Udemy::forward_medium(const ManipParam& param) const {
		TapKey(VK_RIGHT);
		QThread::msleep(REPEAT_WAIT);
		TapKey(VK_RIGHT);
	}
	void Manip_Udemy::backward_medium(const ManipParam& param) const {
		TapKey(VK_LEFT);
		QThread::msleep(REPEAT_WAIT);
		TapKey(VK_LEFT);
	}
	void Manip_Udemy::speedDown(const ManipParam& param) const {
		TapKey(VK_LEFT, VK_SHIFT);
	}
	void Manip_Udemy::speedUp(const ManipParam& param) const {
		TapKey(VK_RIGHT, VK_SHIFT);
	}
	void Manip_Udemy::volumeDown(const ManipParam& param) const {
		TapKey(VK_DOWN);
	}
	void Manip_Udemy::volumeUp(const ManipParam& param) const {
		TapKey(VK_UP);
	}
	void Manip_Udemy::volumeMute(const ManipParam& param) const {
		TapKey(u8'M');
	}
	void Manip_Udemy::fullScreen(const ManipParam& param) const {
		TapKey(u8'F');
	}
	void Manip_Udemy::captionSwitch(const ManipParam& param) const {
		TapKey(u8'C');
	}
	const QString& Manip_Udemy::getName() const {
		static QString Name = QString::fromUtf8(u8"Udemy Manipulator");
		return Name;
	}
}
