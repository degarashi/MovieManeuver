﻿#include "xi_padstate.hpp"
#include <algorithm>
#include <cmath>
#include <QtGlobal>

namespace dg::xinput {
	const int PadState::TRIGGER_RANGE = 255;
	const int PadState::THUMB_RANGE = 32768;
	const int PadState::DEFAULT_DZ_THUMB = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	const int PadState::DEFAULT_DZ_TRIGGER = XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
	namespace {
		constexpr auto ToInt = [](auto&& val) { return static_cast<int>(val); };
	}

	const ButtonState& PadState::_thumbAsButtonState(const Thumb id, const Direction4 dir) const {
		const auto positive = ((dir==Direction4::Right) || (dir==Direction4::Top)) ?
								 AxisState::Dir::Positive : AxisState::Dir::Negative;
		const auto vertical = ToInt(
									((dir==Direction4::Left) || (dir==Direction4::Right)) ?
									Dir2D::Horizontal : Dir2D::Vertical
								);
		return thumb(id).axis(vertical).trigger(positive).buttonState();
	}
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

		_thumb[ToInt(Dir2D::Horizontal)].update(pad.sThumbLX, pad.sThumbLY);
		_thumb[ToInt(Dir2D::Vertical)].update(pad.sThumbRX, pad.sThumbRY);
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
		return _thumb[ThumbId(t)].vec();
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
	const PadState::AxisState2D& PadState::thumb(const Thumb id) const {
		return _thumb[ThumbId(id)];
	}
	PadState::AxisState2D& PadState::refThumb(const Thumb id) {
		return _thumb[ThumbId(id)];
	}
	bool PadState::thumbTilted(const Thumb id, const Direction4 dir) const {
		return _thumbAsButtonState(id, dir).pressed();
	}
	bool PadState::thumbRestored(Thumb id, Direction4 dir) const {
		return _thumbAsButtonState(id, dir).released();
	}
	VKStateAr PadState::getState() const {
		VKStateAr ret;
		{
			struct BtnPair {
				Button 	id;
				VirtualKey		vk;
			};
			const BtnPair Btn[] = {
				{Button::A, VirtualKey::B},
				{Button::B, VirtualKey::A},
				{Button::X, VirtualKey::Y},
				{Button::Y, VirtualKey::X},

				{Button::DPadLeft, VirtualKey::DLeft},
				{Button::DPadUp, VirtualKey::DUp},
				{Button::DPadRight, VirtualKey::DRight},
				{Button::DPadDown, VirtualKey::DDown},

				{Button::LeftShoulder, VirtualKey::L1},
				{Button::LeftThumb, VirtualKey::L3},
				{Button::RightShoulder, VirtualKey::R1},
				{Button::RightThumb, VirtualKey::R3},

				{Button::Start, VirtualKey::Start},
				{Button::Back, VirtualKey::Select},
			};
			for(auto& b : Btn) {
				ret[static_cast<int>(b.vk)] = _button[static_cast<int>(b.id)];
			}
		}
		{
			struct TiltPair {
				Thumb thumb;
				Direction4	dir;
				VirtualKey	vk;
			};
			const TiltPair TP[] = {
				{Thumb::ThumbLeft, Direction4::Left, VirtualKey::TL_Left},
				{Thumb::ThumbLeft, Direction4::Top, VirtualKey::TL_Up},
				{Thumb::ThumbLeft, Direction4::Right, VirtualKey::TL_Right},
				{Thumb::ThumbLeft, Direction4::Bottom, VirtualKey::TL_Down},

				{Thumb::ThumbRight, Direction4::Left, VirtualKey::TR_Left},
				{Thumb::ThumbRight, Direction4::Top, VirtualKey::TR_Up},
				{Thumb::ThumbRight, Direction4::Right, VirtualKey::TR_Right},
				{Thumb::ThumbRight, Direction4::Bottom, VirtualKey::TR_Down},
			};
			for(auto& t : TP) {
				ret[static_cast<int>(t.vk)] =
					_thumbAsButtonState(t.thumb, t.dir);
			}
		}
		{
			const auto procTrigger = [this, &ret](const auto trigger, const auto vk){
				auto& t = getTrigger(Trigger::TriggerLeft).buttonState();
				ret[static_cast<int>(vk)] = getTrigger(trigger).buttonState();
			};
			procTrigger(Trigger::TriggerLeft, VirtualKey::L2);
			procTrigger(Trigger::TriggerRight, VirtualKey::R2);
		}
		return ret;
	}
}
