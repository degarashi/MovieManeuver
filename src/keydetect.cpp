#include "keydetect.hpp"
#include "keyinput.hpp"
#include <cassert>

namespace dg::input {
	// --- KD_Press ---
	KD_Press::KD_Press(VirtualKey key):
		  key(key){}
	bool KD_Press::check(const KeyInput& input, ProcessedKeys& proced) const {
		if(proced.count(key) == 0) {
			if(input.pressed(key, 1)) {
				proced.emplace(key);
				return true;
			}
		}
		return false;
	}

	// --- KI_Double ---
	KD_Double::KD_Double(VirtualKey key, int count): key(key), count(count) {
		assert(count >= 1);
	}
	void KD_Double::nDetect(NDetectAr& dst) const {
		auto& ds = dst[static_cast<int>(key)];
		ds = std::max(ds, count);
	}
	bool KD_Double::check(const KeyInput& input, ProcessedKeys& proced) const {
		if(proced.count(key) == 0) {
			if(input.pressed(key, count)) {
				proced.emplace(key);
				return true;
			}
		}
		return false;
	}

	// --- KI_Step ---
	KD_Step::KD_Step(VirtualKey key0, VirtualKey key1): key{key0, key1} {}
	bool KD_Step::check(const KeyInput& input, ProcessedKeys& proced) const {
		if(proced.count(key[0]) == 0 &&
			proced.count(key[1]) == 0) {
			if(input.pressing(key[0]) &&
				input.pressed(key[1], 1))
			{
				proced.emplace(key[0]);
				proced.emplace(key[1]);
				return true;
			}
		}
		return false;
	}
}
