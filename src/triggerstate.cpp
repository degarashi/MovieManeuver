#include "triggerstate.hpp"
#include "aux_func.hpp"
#include <QtGlobal>

namespace dg {
	TriggerState::TriggerState(const int range, const int deadZone):
		_range(range),
		_deadZone(deadZone)
	{
		init();
		Q_ASSERT(check());
	}
	void TriggerState::init() {
		_bstate.init();
		_trigger = 0.f;
	}
	bool TriggerState::check() const {
		if(_range < 0)
			return false;
		if(!IsInRange(_deadZone, 0, _range))
			return false;
		if(!IsInRange(_trigger, 0.f, 1.f))
			return false;
		return _bstate.check();
	}
	void TriggerState::update() {
		_bstate.update();
	}
	bool TriggerState::update(const int rawValue) {
		const int pv_i = std::max<int>(0, rawValue - _deadZone);
		const float pv = pv_i / static_cast<float>(_range - _deadZone);
		_trigger = pv;
		_bstate.update(pv > 0);
		return rawValue >= 0;
	}
	int TriggerState::deadZone() const {
		return _deadZone;
	}
	void TriggerState::setDeadZone(const int dz) {
		Q_ASSERT(IsInRange(dz, 0, _range));
		_deadZone = dz;
	}
	float TriggerState::trigger() const {
		return _trigger;
	}
	const ButtonState& TriggerState::buttonState() const {
		return _bstate;
	}
}
