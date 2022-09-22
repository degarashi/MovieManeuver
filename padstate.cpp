#include "padstate.hpp"
#include <algorithm>
#include <cmath>
#include <QtGlobal>
#include <QDebug>

namespace {
	template <class T>
	bool IsInRange(const T& val, const T& min, const T& max) {
		return (val >= min) && (val <= max);
	}
}
namespace dg {
	const int PadState::TRIGGER_RANGE = 255;
	const int PadState::THUMB_RANGE = 32768;
	const int PadState::DEFAULT_DZ_THUMB = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	const int PadState::DEFAULT_DZ_TRIGGER = XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

	PadState::PadState() {
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
			t = 0;
		for(auto& t: _thumb)
			t = Vec2{};
	}
    void PadState::updateState() {
		// ボタンやAxisの値は変化させない
		// カウント値だけ更新
		for(auto& btn : _button)
			btn.update();
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

		const auto realValue = [](
			const int rawValue,
			const int deadzone,
			const int maxRange,
			float& processedValueDst
		) {
			Q_ASSERT(deadzone <= maxRange);
			Q_ASSERT(std::abs(rawValue) <= maxRange);

			const int pv_i= std::max<int>(0, std::abs(rawValue) - deadzone);
			const float pv = pv_i / static_cast<float>(maxRange - deadzone);
			processedValueDst = (rawValue < 0) ? -pv: pv;
			return pv > 0;
		};
		// LeftTrigger
		{
			const bool positive = realValue(
				pad.bLeftTrigger,
				getTriggerDeadZone(E_Trigger::TriggerLeft),
				TRIGGER_RANGE,
				_trigger[E_Trigger::TriggerLeft]
			);
			Q_ASSERT(_trigger[E_Trigger::TriggerLeft] >= 0);
			_button[E_Button::LeftTrigger].update(positive);
		}
		// RightTrigger
		{
			const bool positive = realValue(
				pad.bRightTrigger,
				getTriggerDeadZone(E_Trigger::TriggerRight),
				TRIGGER_RANGE,
				_trigger[E_Trigger::TriggerRight]
			);
			Q_ASSERT(_trigger[E_Trigger::TriggerRight] >= 0);
			_button[E_Button::RightTrigger].update(positive);
		}

		const auto procAxis = [this, &realValue](
			const int indexPositive, const int indexNegative,
			float& processedValueDst,
			const int rawValue, const int deadzone, const int max_range)
		{
			const bool bval = realValue(rawValue, deadzone, max_range, processedValueDst);
			bool pos = false,
				neg = false;
			if(rawValue > 0) {
				pos = bval;
			} else if(rawValue < 0) {
				neg = bval;
			}
			_button[indexPositive].update(pos);
			_button[indexNegative].update(neg);
		};
		procAxis(E_Button::LeftThumbPX, E_Button::LeftThumbNX, _thumb[E_Thumb::ThumbLeft].x,
					pad.sThumbLX, getThumbDeadZone(E_Thumb::ThumbLeft), THUMB_RANGE);
		procAxis(E_Button::LeftThumbPY, E_Button::LeftThumbNY, _thumb[E_Thumb::ThumbLeft].y,
					pad.sThumbLY, getThumbDeadZone(E_Thumb::ThumbLeft), THUMB_RANGE);
		procAxis(E_Button::RightThumbPX, E_Button::RightThumbNX, _thumb[E_Thumb::ThumbRight].x,
					pad.sThumbRX, getThumbDeadZone(E_Thumb::ThumbRight), THUMB_RANGE);
		procAxis(E_Button::RightThumbPY, E_Button::RightThumbNY, _thumb[E_Thumb::ThumbRight].y,
					pad.sThumbRY, getThumbDeadZone(E_Thumb::ThumbRight), THUMB_RANGE);
	}
	bool PadState::check() const {
		for(const auto& btn : _button)
			if(!btn.check())
				return false;

		// いずれか片方が0でないとおかしい
		auto axisCheck = [this](const auto id0, const auto id1) {
			return (_button[id0].pressing() * _button[id1].pressing()) == 0;
		};
		if(!axisCheck(E_Button::DPadLeft, E_Button::DPadRight)) return false;
		if(!axisCheck(E_Button::DPadUp, E_Button::DPadDown)) return false;
		if(!axisCheck(E_Button::LeftThumbPX, E_Button::LeftThumbNX)) return false;
		if(!axisCheck(E_Button::LeftThumbPY, E_Button::LeftThumbNY)) return false;
		if(!axisCheck(E_Button::RightThumbPY, E_Button::RightThumbNY)) return false;
		if(!axisCheck(E_Button::RightThumbPY, E_Button::RightThumbNY)) return false;

		// 0以上MAX値以下
		if(!IsInRange(getThumbDeadZone(E_Thumb::ThumbLeft), 0, THUMB_RANGE)) return false;
		if(!IsInRange(getTriggerDeadZone(E_Trigger::TriggerLeft), 0, TRIGGER_RANGE)) return false;
		if(!IsInRange(getThumbDeadZone(E_Thumb::ThumbRight), 0, THUMB_RANGE)) return false;
		if(!IsInRange(getTriggerDeadZone(E_Trigger::TriggerRight), 0, TRIGGER_RANGE)) return false;

		return true;
	}
	bool PadState::pressed(const E_Button id) const {
		return _button[id].pressed();
	}
	bool PadState::released(const E_Button id) const {
		return _button[id].released();
	}
	ButtonState::Frames PadState::pressing(const E_Button id) const {
		return _button[id].pressing();
	}
	float PadState::getTrigger(const E_Trigger t) const {
		return _trigger[t];
	}
	Vec2 PadState::getThumb(const E_Thumb t) const {
		return _thumb[t];
	}
	IVec2 PadState::getDPadVec() const {
		return {
			static_cast<int>(_button[E_Button::DPadRight].pressing() > 0)
				- static_cast<int>(_button[E_Button::DPadLeft].pressing() > 0),
			static_cast<int>(_button[E_Button::DPadUp].pressing() > 0)
				- static_cast<int>(_button[E_Button::DPadDown].pressing() > 0),
		};
	}
	int PadState::getThumbDeadZone(const E_Thumb id) const {
		return _deadzone.thumb[id];
	}
	void PadState::setThumbDeadZone(const E_Thumb id, const int dz) {
		_deadzone.thumb[id] = dz;
		Q_ASSERT(IsInRange(dz, 0, THUMB_RANGE));
	}
	int PadState::getTriggerDeadZone(const E_Trigger id) const {
		return _deadzone.trigger[id];
	}
	void PadState::setTriggerDeadZone(const E_Trigger id, const int dz) {
		_deadzone.trigger[id] = dz;
		Q_ASSERT(IsInRange(dz, 0, TRIGGER_RANGE));
	}
}
