#include "xi_padstate.hpp"
#include <algorithm>
#include <cmath>
#include <QtGlobal>

namespace dg::xinput {
	const int PadState::TRIGGER_RANGE = 255;
	const int PadState::THUMB_RANGE = 32768;
	const int PadState::DEFAULT_DZ_THUMB = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	const int PadState::DEFAULT_DZ_TRIGGER = XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

	PadState::PadState():
		_trigger {
			TriggerState(TRIGGER_RANGE, DEFAULT_DZ_TRIGGER),
			TriggerState(TRIGGER_RANGE, DEFAULT_DZ_TRIGGER)
		},
		_thumb {
			AxisState2D(THUMB_RANGE, DEFAULT_DZ_THUMB),
			AxisState2D(THUMB_RANGE, DEFAULT_DZ_THUMB)
		}
	{
		setNeutral(true);
	}
	void PadState::setNeutral(const bool init) {
		if(!init) {
			updateState(XINPUT_GAMEPAD{});
			return;
		}
		for(auto& btn: _button)
			btn.init();
		for(auto& t: _trigger)
			t.init();
		for(auto& t: _thumb)
			t.init();
	}
	void PadState::updateState() {
		// ボタンやAxisの値は変化させない
		// カウント値だけ更新
		for(auto& btn : _button)
			btn.update();
		for(auto& tri : _trigger)
			tri.update();
		for(auto& t : _thumb)
			t.update();
	}
	void PadState::updateState(const XINPUT_GAMEPAD& pad) {
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
			const bool positive = _trigger[TriggerId(Trigger::TriggerLeft)].update(pad.bLeftTrigger);
			Q_ASSERT(positive);
		}
		// RightTrigger
		{
			const bool positive = _trigger[TriggerId(Trigger::TriggerRight)].update(pad.bRightTrigger);
			Q_ASSERT(positive);
		}

		_thumb[AxisState2D::Horizontal].update(pad.sThumbLX, pad.sThumbLY);
		_thumb[AxisState2D::Vertical].update(pad.sThumbRX, pad.sThumbRY);
	}
	bool PadState::check() const {
		for(const auto& btn : _button)
			if(!btn.check())
				return false;
		for(const auto& tri : _trigger)
			if(!tri.check())
				return false;
		for(const auto& a : _thumb)
			if(!a.check())
				return false;

		// いずれか片方が0でないとおかしい
		auto axisCheck = [this](const Button id0, const Button id1) {
			return (_button[BtnId(id0)].pressing() * _button[BtnId(id1)].pressing()) == 0;
		};
		if(!axisCheck(Button::DPadLeft, Button::DPadRight)) return false;
		if(!axisCheck(Button::DPadUp, Button::DPadDown)) return false;

		return true;
	}
	bool PadState::pressed(const Button id) const {
		return _button[BtnId(id)].pressed();
	}
	bool PadState::released(const Button id) const {
		return _button[BtnId(id)].released();
	}
	ButtonState::Frames PadState::pressing(const Button id) const {
		return _button[BtnId(id)].pressing();
	}
	const TriggerState& PadState::getTrigger(const Trigger t) const {
		return _trigger[TriggerId(t)];
	}
	Vec2 PadState::getThumb(const Thumb t) const {
		return _thumb[ThumbId(t)].dir();
	}
	IVec2 PadState::getDPadVec() const {
		return {
			static_cast<int>(_button[BtnId(Button::DPadRight)].pressing() > 0)
				- static_cast<int>(_button[BtnId(Button::DPadLeft)].pressing() > 0),
			static_cast<int>(_button[BtnId(Button::DPadUp)].pressing() > 0)
				- static_cast<int>(_button[BtnId(Button::DPadDown)].pressing() > 0),
		};
	}
	int PadState::getTriggerDeadZone(const Trigger id) const {
		return _trigger[TriggerId(id)].deadZone();
	}
	void PadState::setTriggerDeadZone(const Trigger id, const int dz) {
		_trigger[TriggerId(id)].setDeadZone(dz);
	}
	const AxisState2D& PadState::thumb(const Thumb id) const {
		return _thumb[ThumbId(id)];
	}
	AxisState2D& PadState::refThumb(const Thumb id) {
		return _thumb[ThumbId(id)];
	}
	bool PadState::thumbTilted(const Thumb id, const Direction4 dir) const {
		const auto positive = ((dir==Direction4::Right) || (dir==Direction4::Top)) ?
								 AxisState::Dir::Positive : AxisState::Dir::Negative;
		const int vertical = ((dir==Direction4::Left) || (dir==Direction4::Right)) ?
								 AxisState2D::Horizontal : AxisState2D::Vertical;
		return thumb(id).axis(vertical).trigger(positive).buttonState().pressed();
	}
}
