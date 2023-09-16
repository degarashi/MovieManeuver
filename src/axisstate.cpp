#include "axisstate.hpp"
#include "aux_func.hpp"
#include <QtGlobal>

namespace dg {
	namespace {
		constexpr auto Negative = static_cast<int>(AxisState::Dir::Negative);
		constexpr auto Positive = static_cast<int>(AxisState::Dir::Positive);
	}
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
	const TriggerState& AxisState::trigger(const Dir dir) const {
		constexpr int N = sizeof(_tstate) / sizeof(_tstate[0]);
		const auto dir_ = static_cast<int>(dir);
		Q_ASSERT(dir_ <= N);
		return _tstate[dir_];
	}
	TriggerState& AxisState::refTrigger(const Dir dir) {
		constexpr int N = sizeof(_tstate) / sizeof(_tstate[0]);
		const auto dir_ = static_cast<int>(dir);
		Q_ASSERT(dir_ <= N);
		return _tstate[dir_];
	}
	float AxisState::axis() const {
		return _axis;
	}
	void AxisState::setDeadZone(const int dz) {
		for(auto& t : _tstate)
			t.setDeadZone(dz);
	}
}
