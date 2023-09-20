#include "keydetect.hpp"
#include "keyinput.hpp"
#include <cassert>

namespace dg::input {
	// --- KD_Press ---
	KD_Press::KD_Press(VirtualKey key):
		  key(key){}
	bool KD_Press::check(const KeyInput& input, ProcessedKeys& proced) const {
		if(!hasChecked(proced)) {
			if(input.pressed(key, 1)) {
				proced.emplace(key);
				return true;
			}
		}
		return false;
	}
	bool KD_Press::hasChecked(const ProcessedKeys& proced) const {
		return proced.count(key) != 0;
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
		if(!hasChecked(proced)) {
			if(input.pressed(key, count)) {
				proced.emplace(key);
				return true;
			}
		}
		return false;
	}
	bool KD_Double::hasChecked(const ProcessedKeys& proced) const {
		return proced.count(key) != 0;
	}

	// --- KI_Simul ---
	void KD_Simul::nDetect(NDetectAr& dst) const {
		return key1->nDetect(dst);
	}
	bool KD_Simul::check(const KeyInput& input, ProcessedKeys& proced) const {
		if(!hasChecked(proced)) {
			if(input.pressing(key0) &&
				key1->check(input, proced))
			{
				proced.emplace(key0);
				return true;
			}
		}
		return false;
	}
	bool KD_Simul::hasChecked(const ProcessedKeys& proced) const {
		return !(proced.count(key0) == 0 && !key1->hasChecked(proced));
	}
}
