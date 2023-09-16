#pragma once
#include "buttonstate.hpp"

namespace dg {
	//! アナログ入力のボタンを管理
	class TriggerState {
		private:
			//! トリガーをボタンとして解釈した場合の情報
			ButtonState		_bstate;
			//! トリガーの押され具合を0.0から1.0までに正規化された値
			float			_trigger;
			int				_range,			//! 最大レンジ
							_deadZone;		//! 反応しない範囲
		public:
			TriggerState(int range, int deadZone);
			//! 明示的な初期化(コンストラクタでも呼ばれる)
			void init();
			//! デバッグ用(内部の値が正常かをチェック)
			[[nodiscard]] bool check() const;

			//! 前回と入力の変化が無いときに呼ぶ
			void update();
			//! rawValueが0以上の時、trueが返る
			bool update(int rawValue);

			// ---- DeadZone ----
			[[nodiscard]] int deadZone() const;
			void setDeadZone(int dz);

			[[nodiscard]] float trigger() const;
			[[nodiscard]] const ButtonState& buttonState() const;
	};
}
