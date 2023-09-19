#pragma once
#include "buttonstate.hpp"
#include "virtual_key.hpp"
#include <array>

namespace dg {
	using VKStateAr = std::array<ButtonState, Num_VirtualKey>;
	using NDetectAr = std::array<int, Num_VirtualKey>;
}
