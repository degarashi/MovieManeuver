#pragma once
#include "axisstate.hpp"
#include "vector.hpp"

namespace dg {
	class AxisState2D {
		public:
			constexpr static int
				Horizontal = 0,
				Vertical = 1;
		private:
			AxisState	_astate[2];
			Vec2		_dir;

		public:
			AxisState2D(int range, int deadZone);
			void init();
			[[nodiscard]] bool check() const;

			// 前回と入力の変化が無いときに呼ぶ
			void update();
			void update(int hValue, int vValue);

			[[nodiscard]] const AxisState& axis(int id) const;
			[[nodiscard]] AxisState& refAxis(int id);
			[[nodiscard]] const Vec2& dir() const;

			void setDeadZone(int dz);
	};
}
