#include "axisstate2d.hpp"

namespace dg {
	AxisState2D::AxisState2D(const int range, const int deadZone):
		_astate {
			AxisState(range, deadZone),
			AxisState(range, deadZone)
		}
	{
		init();
	}
	void AxisState2D::init() {
		for(auto& a : _astate)
			a.init();
		_dir = Vec2{};
	}
	bool AxisState2D::check() const {
		for(auto& a : _astate)
			if(!a.check())
				return false;
		return true;
	}
	void AxisState2D::update() {
		for(auto& a : _astate)
			a.update();
	}
	void AxisState2D::update(const int hValue, const int vValue) {
		auto &h = _astate[Horizontal],
				&v = _astate[Vertical];
		h.update(hValue);
		v.update(vValue);
		_dir.x = h.axis();
		_dir.y = v.axis();
	}
	const AxisState& AxisState2D::axis(const int id) const {
		return _astate[id];
	}
	AxisState& AxisState2D::refAxis(const int id) {
		return _astate[id];
	}
	const Vec2& AxisState2D::dir() const {
		return _dir;
	}
	void AxisState2D::setDeadZone(const int dz) {
		for(auto& a : _astate)
			a.setDeadZone(dz);
	}
}
