#pragma once
#include "buttonstate.hpp"

namespace dg {
	class TriggerState {
		private:
			ButtonState		_bstate;
			// 0.0から1.0までの値
			float			_trigger;
			int				_range,
							_deadZone;
		public:
			TriggerState(int range, int deadZone);
			void init();
			[[nodiscard]] bool check() const;

			// 前回と入力の変化が無いときに呼ぶ
			void update();
			// rawValueが0以上の時、trueが返る
			bool update(int rawValue);

			[[nodiscard]] int deadZone() const;
			void setDeadZone(int dz);

			[[nodiscard]] float trigger() const;
			[[nodiscard]] const ButtonState& buttonState() const;
	};
}
