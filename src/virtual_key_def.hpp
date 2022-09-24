#pragma once
#include "virtual_key.hpp"
#include <windows.h>
#include <vector>
#include <unordered_map>

namespace dg {
	using VKInputs = std::vector<VirtualKey>;
	class Manip;
	using Manip_F = void (Manip::*)(HWND) const;
	using VKMapping = std::unordered_map<VirtualKey, Manip_F>;
}
