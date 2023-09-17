#pragma once
#include "virtual_key.hpp"
#include <vector>

namespace dg {
	struct KeyDiff {
		VirtualKey	key;

		// true: pressed
		// false: released
		bool		pressed;

		KeyDiff(VirtualKey key, bool pressed):
			key(key), pressed(pressed) {}
	};
	using KeyDiff_V = std::vector<KeyDiff>;
}
