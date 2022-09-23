#include "inputmgr.hpp"
#include <QThread>
#include <QTimer>
#include <QDebug>

namespace dg {
	InputMgr::InputMgr():
		_pktNum(0)
	{
		_timer = new QTimer;
		_timer->start(16);
		connect(_timer, &QTimer::timeout, this, &InputMgr::onTimer);

		const int Range = XI_PadState::THUMB_RANGE;
		const int DZ = Range - (Range >> 5);
		_state.refThumb(XI_PadState::E_Thumb::ThumbLeft).setDeadZone(DZ);
		_state.refThumb(XI_PadState::E_Thumb::ThumbRight).setDeadZone(DZ);
	}

	void InputMgr::onTimer() {
		XINPUT_STATE xstate{};
		if(XInputGetState(0, &xstate) == ERROR_SUCCESS) {
			if(_pktNum == xstate.dwPacketNumber) {
				_state.updateState();
			} else {
				_pktNum = xstate.dwPacketNumber;
				_state.updateState(xstate.Gamepad);
			}
			Q_ASSERT(_state.check());
			emit onInput(_state);
		}
	}
}
