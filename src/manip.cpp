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

	void Manip::mediaVolumeUp(const ManipParam& param) const {
		Q_UNUSED(param)
		TapKey(VK_VOLUME_UP);
	}
	void Manip::mediaVolumeDown(const ManipParam& param) const {
		Q_UNUSED(param)
		TapKey(VK_VOLUME_DOWN);
	}
}
