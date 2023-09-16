#pragma once
#include "axisstate.hpp"
#include "vector.hpp"

namespace dg {
	class AxisState2D {
		public:
			enum class Dir {
				Horizontal,
				Vertical,
				_Num
			};
		private:
			AxisState	_astate[static_cast<int>(Dir::_Num)];
			Vec2		_dir;

		public:
			AxisState2D(int range, int deadZone);
			void init();
			[[nodiscard]] bool check() const;

			// 前回と入力の変化が無いときに呼ぶ
			void update();
			void update(int hValue, int vValue);

			[[nodiscard]] const AxisState& axis(Dir dir) const;
			[[nodiscard]] AxisState& refAxis(Dir dir);
			[[nodiscard]] const Vec2& dir() const;

			void setDeadZone(int dz);
	};
}
