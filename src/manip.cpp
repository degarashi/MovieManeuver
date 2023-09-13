#include "manip.hpp"
#include "winop.hpp"
#include <QThread>

namespace dg {
	HWND Manip::findTarget() const {
		dg::FindWindowParam param = {};
		param.targetTopName = getWindowTopName();
		param.targetChildName = getWindowChildName();
		EnumWindows(dg::WndEnumProc, reinterpret_cast<LPARAM>(&param));
		return param.result;
	}

	void Manip::mediaVolumeUp(HWND hw) const {
		Q_UNUSED(hw)
		TapKey(VK_VOLUME_UP);
	}
	void Manip::mediaVolumeDown(HWND hw) const {
		Q_UNUSED(hw)
		TapKey(VK_VOLUME_DOWN);
	}
}
