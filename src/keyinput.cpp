#include "keyinput.hpp"

namespace dg::input {
	KeyInput::NPress::NPress() {
		resetAccum();
		result = 0;
	}
	void KeyInput::NPress::resetAccum() {
		count = 0;
		accumTime = MilliSec(0);
	}
	void KeyInput::NPress::update(MilliSec dt, bool press, int maxCount) {
		result = 0;
		// 時間加算と期限切れ判定
		if(count > 0)
			accumTime += dt;
		if(accumTime >= MaxDelay) {
			result = count;
			resetAccum();
		} else {
			// カウンタの一巡判定
			if(press) {
				if(++count >= maxCount) {
					result = count;
					resetAccum();
				}
			}
		}
	}

	// --- KeyInput ---
	void KeyInput::update(const VKStateAr& state, const MilliSec dt, const NDetectAr& maxLayer) {
		for(int i=0 ; i<Num_VirtualKey ; i++) {
			_pressing[i] = state[i].pressing();
			nPress[i].update(dt, state[i].pressed(), maxLayer[i]);
		}
	}
	bool KeyInput::pressed(const VirtualKey key, const int n) const {
		return nPress[static_cast<int>(key)].result == n;
	}

	bool KeyInput::pressing(VirtualKey key) const {
		return _pressing[static_cast<int>(key)];
	}
}
