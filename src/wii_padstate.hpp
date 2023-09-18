#pragma once
#include "virtual_key.hpp"
#include "buttonstate.hpp"
#include "wii_enums.hpp"
#include <unordered_map>
#include "axisstate_n.hpp"
#include "keydiff.hpp"

struct wrmt_wiiremote;
using WRMT_WiiRemote = wrmt_wiiremote;
namespace dg::wii {
	using ButtonToVKMap_t = std::unordered_map<Button, VirtualKey>;
	using VKToButtonMap_t = std::unordered_map<VirtualKey, Button>;
	extern const ButtonToVKMap_t ButtonToVKMap;
	extern const VKToButtonMap_t VKToButtonMap;

	// WiiMote用のPadState
	class Remote {
		private:
			WRMT_WiiRemote*	_data;
			using ButtonState_a = std::array<ButtonState, static_cast<size_t>(Button::_Num)>;
			using Axis3D = AxisState_N<3>;
			ButtonState_a	_bstate;
			Axis3D			_accel;

			using BoolAr = std::array<bool, static_cast<size_t>(Button::_Num)>;
			[[nodiscard]] BoolAr _getPressingButton() const;

		public:
			Remote(Remote&& rmt);
			Remote(const Remote&) = delete;
			Remote(WRMT_WiiRemote* data);
			~Remote();
			[[nodiscard]] KeyDiff_V getButtonDiff() const;
			[[nodiscard]] KeyDiff_V getAccelDiff() const;
			[[nodiscard]] const Axis3D& getAcceleration() const;

			void updateState();
			// 前回から何も変化が無いときに呼ぶ
			void updateKeepState();
	};
}
