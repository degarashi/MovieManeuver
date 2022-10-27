#include "manip_vlc.hpp"
#include "winop.hpp"
#include <QThread>
#include <QDateTime>

namespace dg {
	namespace {
		constexpr unsigned long MANIP_WAIT = 25;
		constexpr unsigned long VOLUME_MANIP_OFFSET = 160;
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
		TempSwitch(hw, [](){
			TapKey(VK_SPACE);
		}, MANIP_WAIT);
	}
	void Manip_VLC::forward_5sec(const HWND hw) const {
		TempSwitch(hw, [](){
			TapKey(VK_RIGHT, VK_SHIFT);
		}, MANIP_WAIT);
	}
	void Manip_VLC::backward_5sec(const HWND hw) const {
		TempSwitch(hw, [](){
			TapKey(VK_LEFT, VK_SHIFT);
		}, MANIP_WAIT);
	}
	void Manip_VLC::forward_10sec(const HWND hw) const {
		TempSwitch(hw, [](){
			TapKey(VK_RIGHT, VK_MENU);
		}, MANIP_WAIT);
	}
	void Manip_VLC::backward_10sec(const HWND hw) const {
		TempSwitch(hw, [](){
			TapKey(VK_LEFT, VK_MENU);
		}, MANIP_WAIT);
	}
	void Manip_VLC::speedDown(const HWND hw) const {
		TempSwitch(hw, [](){
			TapKey(VK_OEM_4);	// [
		}, MANIP_WAIT);
	}
	void Manip_VLC::speedUp(const HWND hw) const {
		TempSwitch(hw, [](){
			TapKey(VK_OEM_6);	// ]
		}, MANIP_WAIT);
	}
	void Manip_VLC::volumeDown(const HWND hw) const {
		TempSwitch(hw, [](){
			TapKey(VK_DOWN, VK_CONTROL);
		}, MANIP_WAIT);
	}
	void Manip_VLC::volumeUp(const HWND hw) const {
		TempSwitch(hw, [](){
			TapKey(VK_UP, VK_CONTROL);
		}, MANIP_WAIT);
	}
	void Manip_VLC::volumeMute(const HWND hw) const {
		TempSwitch(hw, [](){
			TapKey(u8'M');
		}, MANIP_WAIT);
	}
	void Manip_VLC::captionSwitch(const HWND hw) const {
		TempSwitch(hw, [](){
			TapKey(u8'V');
		}, MANIP_WAIT);
	}
	const QString& Manip_VLC::getName() const {
		static QString Name = QString::fromUtf8(u8"VLC Manipulator");
		return Name;
	}
}
