#include "wii_padstate.hpp"
#include "wii_exception.hpp"
#include <QtGlobal>
#include <wiiremote.h>

namespace dg {
	namespace wii {
		const ButtonToVKMap_t ButtonToVKMap = {
			{Button::A, VirtualKey::A},
			{Button::B, VirtualKey::B},
			{Button::One, VirtualKey::X},
			{Button::Two, VirtualKey::Y},
			{Button::Minus, VirtualKey::Select},
			{Button::Home, VirtualKey::L1},
			{Button::Plus, VirtualKey::Start},
			{Button::Left, VirtualKey::DLeft},
			{Button::Up, VirtualKey::DUp},
			{Button::Right, VirtualKey::DRight},
			{Button::Down, VirtualKey::DDown},
		};

		Remote::Remote(Remote&& rmt):
			_data(rmt._data),
			_bstate(rmt._bstate),
			_accel(rmt._accel)
		{
			rmt._data = nullptr;
		}
		namespace {
			constexpr int DeadZone = 19,
							Range = 128;
		}
		// Accelの範囲はUnsigned(0～255), 中央値=128
		Remote::Remote(WRMT_WiiRemote* data):
			_data(data),
			_accel(Range, DeadZone)
		{
			Q_ASSERT(data);

			const WRMT_IOReturn rc = WRMT_WiiRemote_Open(data);
			if(rc == WRMT_IO_ERROR)
				throw Exception("WRMT_WiiRemote_Open() Failed");

			// モーションセンサはON
			WRMT_WiiRemote_SetEnabled(_data, WRMT_FUNCTION_MOTION, 1);
			// その他余計な機能をOFF
			WRMT_WiiRemote_SetEnabled(_data, WRMT_FUNCTION_CONTINUOUS, 0);
			WRMT_WiiRemote_SetEnabled(_data, WRMT_FUNCTION_IR, 0);
			WRMT_WiiRemote_SetEnabled(_data, WRMT_FUNCTION_SPEAKER, 0);
			WRMT_WiiRemote_Update(_data);
		}
		Remote::~Remote() {
			WRMT_WiiRemote_Close(_data);
		}

		namespace {
			using MaskToButtonPair = std::pair<uint16_t, Button>;
			const MaskToButtonPair Mask2B[static_cast<size_t>(Button::_Num)] = {
				{WRMT_MASK_BUTTON_A, Button::A},
				{WRMT_MASK_BUTTON_B, Button::B},
				{WRMT_MASK_BUTTON_ONE, Button::One},
				{WRMT_MASK_BUTTON_TWO, Button::Two},
				{WRMT_MASK_BUTTON_MINUS, Button::Minus},
				{WRMT_MASK_BUTTON_PLUS, Button::Plus},
				{WRMT_MASK_BUTTON_HOME, Button::Home},
				{WRMT_MASK_BUTTON_UP, Button::Up},
				{WRMT_MASK_BUTTON_LEFT, Button::Left},
				{WRMT_MASK_BUTTON_RIGHT, Button::Right},
				{WRMT_MASK_BUTTON_DOWN, Button::Down},
			};
		}
		BoolAr Remote::getPressingButton() const {
			BoolAr ret = {};
			const int bs = WRMT_WiiRemote_GetState(_data, WRMT_DATA_BUTTONS);
			for(auto& msk : Mask2B) {
				ret[static_cast<int>(msk.second)] = (bs & msk.first);
			}
			return ret;
		}

		const Remote::Axis3D& Remote::getAcceleration() const {
			return _accel;
		}
		void Remote::updateState() {
			// Buttons
			const auto pressing = getPressingButton();
			for(int i=0 ; i<static_cast<int>(Button::_Num) ; i++) {
				_bstate[i].update(pressing[i]);
			}
			// Acceleration
			{
				const int x = WRMT_WiiRemote_GetState(_data, WRMT_DATA_MOTION_X),
						y = WRMT_WiiRemote_GetState(_data, WRMT_DATA_MOTION_Y),
						z = WRMT_WiiRemote_GetState(_data, WRMT_DATA_MOTION_Z);
				_accel.update(
					x - Range,
					y - Range,
					z - Range
				);
				Q_ASSERT(_accel.check());
			}
		}
		void Remote::updateKeepState() {
			for(auto& bs : _bstate)
				bs.update();
		}
		VKInputs Remote::getPressedButton() const {
			VKInputs vk;
			for(int i=0 ; i<static_cast<int>(Button::_Num) ; i++) {
				if(_bstate[i].pressed()) {
					vk.emplace_back(ButtonToVKMap.at(Button(i)));
				}
			}
			return vk;
		}
	}
}
