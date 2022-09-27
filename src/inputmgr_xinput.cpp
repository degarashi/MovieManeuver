#include "inputmgr_xinput.hpp"
#include "dg_diag_xinput.hpp"

namespace dg::xinput {
	Manager::Manager() {
		const int Range = PadState::THUMB_RANGE;
		const int DZ = Range - (Range >> 5);
		_state.refThumb(PadState::Thumb::ThumbLeft).setDeadZone(DZ);
		_state.refThumb(PadState::Thumb::ThumbRight).setDeadZone(DZ);
	}
	QWidget* Manager::makeDialog() {
		auto* diag = new DebugViewWidget();
		connect(this, &Manager::onInputXI, diag, &DebugViewWidget::updateDebugView);
		return diag;
	}
	VKInputs Manager::_composeInputs() const {
		VKInputs ret;
		{
			struct BtnPair {
				PS::Button 	id;
				VirtualKey		vk;
			};
			const BtnPair Btn[] = {
				{PS::Button::A, VirtualKey::B},
				{PS::Button::B, VirtualKey::A},
				{PS::Button::X, VirtualKey::Y},
				{PS::Button::Y, VirtualKey::X},

				{PS::Button::DPadLeft, VirtualKey::DLeft},
				{PS::Button::DPadUp, VirtualKey::DUp},
				{PS::Button::DPadRight, VirtualKey::DRight},
				{PS::Button::DPadDown, VirtualKey::DDown},

				{PS::Button::LeftShoulder, VirtualKey::L1},
				{PS::Button::LeftThumb, VirtualKey::L3},
				{PS::Button::RightShoulder, VirtualKey::R1},
				{PS::Button::RightThumb, VirtualKey::R3},

				{PS::Button::Start, VirtualKey::Start},
				{PS::Button::Back, VirtualKey::Select},
			};
			for(auto& b : Btn) {
				if(_state.pressed(b.id))
					ret.emplace_back(b.vk);
			}
		}
		{
			struct TiltPair {
				PS::Thumb thumb;
				Direction4	dir;
				VirtualKey	vk;
			};
			const TiltPair TP[] = {
				{PS::Thumb::ThumbLeft, Direction4::Left, VirtualKey::TL_Left},
				{PS::Thumb::ThumbLeft, Direction4::Top, VirtualKey::TL_Up},
				{PS::Thumb::ThumbLeft, Direction4::Right, VirtualKey::TL_Right},
				{PS::Thumb::ThumbLeft, Direction4::Bottom, VirtualKey::TL_Down},

				{PS::Thumb::ThumbRight, Direction4::Left, VirtualKey::TR_Left},
				{PS::Thumb::ThumbRight, Direction4::Top, VirtualKey::TR_Up},
				{PS::Thumb::ThumbRight, Direction4::Right, VirtualKey::TR_Right},
				{PS::Thumb::ThumbRight, Direction4::Bottom, VirtualKey::TR_Down},
			};
			for(auto& t : TP) {
				if(_state.thumbTilted(t.thumb, t.dir))
					ret.emplace_back(t.vk);
			}
		}
		if(_state.getTrigger(PS::Trigger::TriggerLeft).buttonState().pressed())
			ret.emplace_back(VirtualKey::L2);
		if(_state.getTrigger(PS::Trigger::TriggerRight).buttonState().pressed())
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
