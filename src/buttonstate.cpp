#include "buttonstate.hpp"
#include <cmath>
#include <QtGlobal>

namespace dg {
	ButtonState::ButtonState() {
		init();
	}
	void ButtonState::init() {
		_state = false;
		_counter = 0;
	}
	void ButtonState::update() {
		// カウント値だけ更新
		auto& c = _counter;
		if(_state) {
			Q_ASSERT(c > 0);
			++c;
		} else {
			Q_ASSERT(c <= 0);
			c = 0;
		}
	}
	void ButtonState::update(const bool cur) {
		auto& c = _counter;
		if(!_state) {
			if(!cur) {
				Q_ASSERT(c <= 0);
				c = 0;
			} else {
				if(c < 0) {
					Q_ASSERT(c == -1);
					c = 1;
				} else {
					Q_ASSERT(c == 0);
					++c;
				}
			}
		} else {
			if(!cur) {
				Q_ASSERT(c > 0);
				c = -1;
			}
			else {
				Q_ASSERT(c > 0);
				++c;
			}
		}
		_state = cur;
	}
	bool ButtonState::check() const {
		return _counter >= -1;
	}
	bool ButtonState::pressed() const {
		return _counter == 1;
	}
	bool ButtonState::released() const {
		return _counter == -1;
	}
	ButtonState::Frames ButtonState::pressing() const {
		return std::max<Frames>(0, _counter);
	}
}
