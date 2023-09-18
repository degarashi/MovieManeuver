#include "inputmgr_xinput.hpp"
#include "dg_diag_xinput.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace dg::xinput {
	Manager::Manager() {
		const int Range = PadState::THUMB_RANGE;
		const int DZ = Range - (Range >> 5);
		_state.refThumb(Thumb::ThumbLeft).setDeadZone(DZ);
		_state.refThumb(Thumb::ThumbRight).setDeadZone(DZ);
	}
	QWidget* Manager::makeDialog() {
		auto* diag = new DebugViewWidget();
		connect(this, &Manager::onInputXI, diag, &DebugViewWidget::updateDebugView);
		return diag;
	}
	void Manager::onTimer() {
		XINPUT_STATE xstate{};
		if(XInputGetState(0, &xstate) == ERROR_SUCCESS) {
			if(_pktNum == xstate.dwPacketNumber) {
				_state.updateState();
			} else {
				_pktNum = xstate.dwPacketNumber;
				_state.updateState(xstate.Gamepad);
				// 入力に変化があった時だけシグナルを発する
				emit onInput(_state.getState());
				emit onInputXI(_state);
			}
			Q_ASSERT(_state.check());
		}
	}
}
