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
	KeyDiff_V Manager::_composeInputs() const {
		KeyDiff_V ret;
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
				if(_state.pressed(b.id))
					ret.emplace_back(b.vk, true);
				else if(_state.pressed(b.id))
					ret.emplace_back(b.vk, false);
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
				if(_state.thumbTilted(t.thumb, t.dir))
					ret.emplace_back(t.vk, true);
				else if(_state.thumbRestored(t.thumb, t.dir)) {
					ret.emplace_back(t.vk, false);
				}
			}
		}
		{
			const auto procTrigger = [this, &ret](const auto trigger, const auto vk){
				auto& t = _state.getTrigger(Trigger::TriggerLeft).buttonState();
				std::optional<bool> b;
				if(t.pressed())
					b = true;
				else if(t.released())
					b = false;
				if(b)
					ret.emplace_back(VirtualKey::L2, *b);

			};
			procTrigger(Trigger::TriggerLeft, VirtualKey::L2);
			procTrigger(Trigger::TriggerRight, VirtualKey::R2);
		}

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
