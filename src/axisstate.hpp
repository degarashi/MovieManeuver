#pragma once
#include "triggerstate.hpp"

namespace dg {
	//! 軸入力を管理(1D)
	class AxisState {
		public:
			enum class Dir {
				Negative,
				Positive,
				_Num
			};
		private:
			// TriggerStateをPositive/Negativeで2つ用意
			TriggerState	_tstate[static_cast<int>(Dir::_Num)];
			//! 軸入力を-1.0から1.0までの値に正規化した値
			float			_axis;

		public:
			AxisState(int range, int deadZone);
			//! 明示的な初期化(コンストラクタでも呼ばれる)
			void init();
			//! デバッグ用(内部の値が正常かをチェック)
			[[nodiscard]] bool check() const;

			// --- マイフレーム呼ぶ関数 ---
			//! 前回と入力の変化が無いときに呼ぶ
			void update();
			//! 内部ステートの更新
			void update(int rawValue);
			// ----------------------------

			[[nodiscard]] const TriggerState& trigger(Dir dir) const;
			[[nodiscard]] TriggerState& refTrigger(Dir dir);
			[[nodiscard]] float axis() const;

			void setDeadZone(int dz);
	};
}
