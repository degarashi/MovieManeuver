#include "inputmgr_xinput.hpp"
#include "dg_diag_xinput.hpp"

namespace dg::xinput {
	Manager::Manager() {
		const int Range = PadState::THUMB_RANGE;
		const int DZ = Range - (Range >> 5);
		_state.refThumb(PadState::E_Thumb::ThumbLeft).setDeadZone(DZ);
		_state.refThumb(PadState::E_Thumb::ThumbRight).setDeadZone(DZ);
	}
	QWidget* Manager::makeDialog() {
		auto* diag = new Diag_XInput();
		connect(this, &Manager::onInputXI, diag, &Diag_XInput::updateDebugView);
		return diag;
	}
	VKInputs Manager::_composeInputs() const {
		VKInputs ret;
		{
			struct BtnPair {
				PS::E_Button 	id;
				VirtualKey		vk;
			};
			const BtnPair Btn[] = {
				{PS::E_Button::A, VirtualKey::B},
				{PS::E_Button::B, VirtualKey::A},
				{PS::E_Button::X, VirtualKey::Y},
				{PS::E_Button::Y, VirtualKey::X},

				{PS::E_Button::DPadLeft, VirtualKey::DLeft},
				{PS::E_Button::DPadUp, VirtualKey::DUp},
				{PS::E_Button::DPadRight, VirtualKey::DRight},
				{PS::E_Button::DPadDown, VirtualKey::DDown},

				{PS::E_Button::LeftShoulder, VirtualKey::L1},
				{PS::E_Button::LeftThumb, VirtualKey::L3},
				{PS::E_Button::RightShoulder, VirtualKey::R1},
				{PS::E_Button::RightThumb, VirtualKey::R3},

				{PS::E_Button::Start, VirtualKey::Start},
				{PS::E_Button::Back, VirtualKey::Select},
			};
			for(auto& b : Btn) {
				if(_state.pressed(b.id))
					ret.emplace_back(b.vk);
			}
		}
		{
			struct TiltPair {
				PS::E_Thumb thumb;
				Direction4	dir;
				VirtualKey	vk;
			};
			const TiltPair TP[] = {
				{PS::E_Thumb::ThumbLeft, Direction4::Left, VirtualKey::TL_Left},
				{PS::E_Thumb::ThumbLeft, Direction4::Top, VirtualKey::TL_Up},
				{PS::E_Thumb::ThumbLeft, Direction4::Right, VirtualKey::TL_Right},
				{PS::E_Thumb::ThumbLeft, Direction4::Bottom, VirtualKey::TL_Down},

				{PS::E_Thumb::ThumbRight, Direction4::Left, VirtualKey::TR_Left},
				{PS::E_Thumb::ThumbRight, Direction4::Top, VirtualKey::TR_Up},
				{PS::E_Thumb::ThumbRight, Direction4::Right, VirtualKey::TR_Right},
				{PS::E_Thumb::ThumbRight, Direction4::Bottom, VirtualKey::TR_Down},
			};
			for(auto& t : TP) {
				if(_state.thumbTilted(t.thumb, t.dir))
					ret.emplace_back(t.vk);
			}
		}
		if(_state.getTrigger(PS::E_Trigger::TriggerLeft).buttonState().pressed())
			ret.emplace_back(VirtualKey::L2);
		if(_state.getTrigger(PS::E_Trigger::TriggerRight).buttonState().pressed())
			ret.emplace_back(VirtualKey::R2);

		return ret;
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
				emit onInput(_composeInputs());
				emit onInputXI(_state);
			}
			Q_ASSERT(_state.check());
		}
	}
}
