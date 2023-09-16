#include "axisstate.hpp"
#include "aux_func.hpp"
#include <QtGlobal>

namespace dg {
	AxisState::AxisState(const int range, const int deadZone):
		_tstate {
			TriggerState(range, deadZone),
			TriggerState(range, deadZone)
		}
	{}
	void AxisState::init() {
		for(auto& t : _tstate)
			t.init();
		_axis = 0.f;
	}
	bool AxisState::check() const {
		for(auto& t : _tstate) {
			if(!t.check())
				return false;
		}
		// いずれか片方が0でないとおかしい
		if((_tstate[Negative].buttonState().pressing()
			* _tstate[Positive].buttonState().pressing()) != 0)
			return false;
		return IsInRange(_axis, -1.f, 1.f);
	}
	void AxisState::update() {
		for(auto& t : _tstate) {
			t.update();
		}
	}
	void AxisState::update(const int rawValue) {
		if(_tstate[Negative].update(-rawValue)) {
			_tstate[Positive].update(0);
			_axis = -_tstate[Negative].trigger();
		} else {
			Q_ASSERT(rawValue > 0);
			const bool b = _tstate[Positive].update(rawValue);
			Q_ASSERT(b);
			_axis = _tstate[Positive].trigger();
		}
	}
	const TriggerState& AxisState::trigger(const int id) const {
		constexpr int N = sizeof(_tstate) / sizeof(_tstate[0]);
		Q_ASSERT(id <= N);
		return _tstate[id];
	}
	TriggerState& AxisState::refTrigger(int id) {
		constexpr int N = sizeof(_tstate) / sizeof(_tstate[0]);
		Q_ASSERT(id <= N);
		return _tstate[id];
	}
	float AxisState::axis() const {
		return _axis;
	}
	void AxisState::setDeadZone(const int dz) {
		for(auto& t : _tstate)
			t.setDeadZone(dz);
	}
}
