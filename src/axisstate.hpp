#pragma once
#include "triggerstate.hpp"

namespace dg {
	class AxisState {
		public:
			constexpr static int
				Negative = 0,
				Positive = 1;
		private:
			TriggerState	_tstate[2];
			// -1.0から1.0までの値
			float			_axis;

		public:
			AxisState(int range, int deadZone);
			void init();
			[[nodiscard]] bool check() const;

			// 前回と入力の変化が無いときに呼ぶ
			void update();
			void update(int rawValue);

			[[nodiscard]] const TriggerState& trigger(int id) const;
			[[nodiscard]] TriggerState& refTrigger(int id);
			[[nodiscard]] float axis() const;

			void setDeadZone(int dz);
	};
}
