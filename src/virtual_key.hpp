#pragma once
#include <cstdint>

namespace dg {
	enum class VirtualKey : uint16_t {
		A, B, X, Y,
		L1, L2, L3,
		R1, R2, R3,
		DLeft, DUp, DRight, DDown,
		TL_Left, TL_Up, TL_Right, TL_Down,
		TR_Left, TR_Up, TR_Right, TR_Down,
		Start, Select
	};
}
