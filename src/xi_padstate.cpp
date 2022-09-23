#include "xi_padstate.hpp"
#include <algorithm>
#include <cmath>
#include <QtGlobal>

namespace dg {
	const int XI_PadState::TRIGGER_RANGE = 255;
	const int XI_PadState::THUMB_RANGE = 32768;
	const int XI_PadState::DEFAULT_DZ_THUMB = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	const int XI_PadState::DEFAULT_DZ_TRIGGER = XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

	XI_PadState::XI_PadState():
		_trigger {
			TriggerState(TRIGGER_RANGE, DEFAULT_DZ_TRIGGER),
			TriggerState(TRIGGER_RANGE, DEFAULT_DZ_TRIGGER)
		},
		_axis {
			AxisState2D(THUMB_RANGE, DEFAULT_DZ_THUMB),
			AxisState2D(THUMB_RANGE, DEFAULT_DZ_THUMB)
		}
	{
		setNeutral(true);
	}
	void XI_PadState::setNeutral(const bool init) {
		if(!init) {
			updateState(XINPUT_GAMEPAD{});
			return;
		}
		for(auto& btn: _button)
			btn.init();
		for(auto& t: _trigger)
			t.init();
		for(auto& a: _axis)
			a.init();
	}
	void XI_PadState::updateState() {
		// ボタンやAxisの値は変化させない
		// カウント値だけ更新
		for(auto& btn : _button)
			btn.update();
		for(auto& tri : _trigger)
			tri.update();
		for(auto& a : _axis)
			a.update();
	}
	void XI_PadState::updateState(const XINPUT_GAMEPAD& pad) {
		// ---- ボタンカウンタの更新 ----
		const int XInput_Flag[] = {
			XINPUT_GAMEPAD_START, XINPUT_GAMEPAD_BACK,
			XINPUT_GAMEPAD_A, XINPUT_GAMEPAD_B, XINPUT_GAMEPAD_X, XINPUT_GAMEPAD_Y,
			XINPUT_GAMEPAD_LEFT_THUMB, XINPUT_GAMEPAD_LEFT_SHOULDER,
			XINPUT_GAMEPAD_RIGHT_THUMB, XINPUT_GAMEPAD_RIGHT_SHOULDER,
			XINPUT_GAMEPAD_DPAD_LEFT, XINPUT_GAMEPAD_DPAD_UP, XINPUT_GAMEPAD_DPAD_RIGHT, XINPUT_GAMEPAD_DPAD_DOWN,
		};
		for(int i=0 ; i<int(std::size(XInput_Flag)) ; i++) {
			const bool cur = pad.wButtons & XInput_Flag[i];
			_button[i].update(cur);
		}

		// LeftTrigger
		{
			const bool positive = _trigger[E_Trigger::TriggerLeft].update(pad.bLeftTrigger);
			Q_ASSERT(positive);
		}
		// RightTrigger
		{
			const bool positive = _trigger[E_Trigger::TriggerRight].update(pad.bRightTrigger);
			Q_ASSERT(positive);
		}

		_axis[AxisState2D::Horizontal].update(pad.sThumbLX, pad.sThumbLY);
		_axis[AxisState2D::Vertical].update(pad.sThumbRX, pad.sThumbRY);
	}
	bool XI_PadState::check() const {
		for(const auto& btn : _button)
			if(!btn.check())
				return false;
		for(const auto& tri : _trigger)
			if(!tri.check())
				return false;
		for(const auto& a : _axis)
			if(!a.check())
				return false;

		// いずれか片方が0でないとおかしい
		auto axisCheck = [this](const auto id0, const auto id1) {
			return (_button[id0].pressing() * _button[id1].pressing()) == 0;
		};
		if(!axisCheck(E_Button::DPadLeft, E_Button::DPadRight)) return false;
		if(!axisCheck(E_Button::DPadUp, E_Button::DPadDown)) return false;

		return true;
	}
	bool XI_PadState::pressed(const E_Button id) const {
		return _button[id].pressed();
	}
	bool XI_PadState::released(const E_Button id) const {
		return _button[id].released();
	}
	ButtonState::Frames XI_PadState::pressing(const E_Button id) const {
		return _button[id].pressing();
	}
	const TriggerState& XI_PadState::getTrigger(const E_Trigger t) const {
		return _trigger[t];
	}
	Vec2 XI_PadState::getThumb(const E_Thumb t) const {
		return _axis[t].dir();
	}
	IVec2 XI_PadState::getDPadVec() const {
		return {
			static_cast<int>(_button[E_Button::DPadRight].pressing() > 0)
				- static_cast<int>(_button[E_Button::DPadLeft].pressing() > 0),
			static_cast<int>(_button[E_Button::DPadUp].pressing() > 0)
				- static_cast<int>(_button[E_Button::DPadDown].pressing() > 0),
		};
	}
	int XI_PadState::getTriggerDeadZone(const E_Trigger id) const {
		return _trigger[id].deadZone();
	}
	void XI_PadState::setTriggerDeadZone(const E_Trigger id, const int dz) {
		_trigger[id].setDeadZone(dz);
	}
	const AxisState2D& XI_PadState::axis(const E_Thumb id) const {
		return _axis[id];
	}
	AxisState2D& XI_PadState::refAxis(const E_Thumb id) {
		return _axis[id];
	}
}
