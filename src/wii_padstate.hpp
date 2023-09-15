#pragma once
#include "virtual_key.hpp"
#include "buttonstate.hpp"
#include "wii_enums.hpp"
#include "virtual_key_def.hpp"
#include <unordered_map>

struct wrmt_wiiremote;
using WRMT_WiiRemote = wrmt_wiiremote;
namespace dg::wii {
	using ButtonToVKMap_t = std::unordered_map<Button, VirtualKey>;
	extern const ButtonToVKMap_t ButtonToVKMap;
	using BoolAr = std::array<bool, static_cast<size_t>(Button::_Num)>;

	// WiiMote用のPadState
	class Remote {
		private:
			WRMT_WiiRemote*	_data;
			using ButtonState_a = std::array<ButtonState, static_cast<size_t>(Button::_Num)>;
			ButtonState_a	_bstate;

		public:
			Remote(Remote&& rmt);
			Remote(const Remote&) = delete;
			Remote(WRMT_WiiRemote* data);
			~Remote();
			[[nodiscard]] VKInputs getPressedButton() const;
			[[nodiscard]] BoolAr getPressingButton() const;

			void updateState();
			// 前回から何も変化が無いときに呼ぶ
			void updateKeepState();
	};
}
