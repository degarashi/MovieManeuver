#pragma once
#include <cstdint>

namespace dg::wii {
	enum class Button {
		A, B,
		One, Two,
		Minus, Home, Plus,
		Left, Up, Right, Down,
		_Num
	};
	enum class Accel {
		X, Y, Z
	};
	constexpr inline auto Num_Button = static_cast<size_t>(Button::_Num);
}
