#pragma once
#include "virtual_key.hpp"
#include "buttonstate.hpp"
#include "wii_enums.hpp"
#include <stdexcept>
#include <unordered_map>

struct wrmt_wiiremote;
using WRMT_WiiRemote = wrmt_wiiremote;
namespace dg::wii {
	class Exception : public std::runtime_error {
		using runtime_error::runtime_error;
	};
	class Init_Fail : public Exception {
		using Exception::Exception;
	};
	using ButtonToVKMap_t = std::unordered_map<Button, VirtualKey>;
	extern const ButtonToVKMap_t ButtonToVKMap;
	using ButtonAr = std::array<bool, static_cast<size_t>(Button::_Num)>;

	// WiiMote用のPadState
	class Remote {
		private:
			WRMT_WiiRemote*	_data;
			using ButtonState_a = std::array<ButtonState, static_cast<size_t>(Button::_Num)>;
			ButtonState_a	_bstate;
			using MaskToButtonPair = std::pair<uint16_t, Button>;
			const static MaskToButtonPair Mask2B[static_cast<size_t>(Button::_Num)];

		public:
			Remote(Remote&& rmt);
			Remote(const Remote&) = delete;
			Remote(WRMT_WiiRemote* data);
			~Remote();
			ButtonAr getPressedButton() const;
	};
}
