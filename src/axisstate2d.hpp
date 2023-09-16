#pragma once
#include "axisstate.hpp"
#include "vector.hpp"

namespace dg {
	//! 軸入力を管理(2D)
	class AxisState2D {
		public:
			enum class Dir {
				Horizontal,
				Vertical,
				_Num
			};
		private:
			AxisState	_astate[static_cast<int>(Dir::_Num)];
			//! 軸入力を最大長さ1のベクトルにした値
			Vec2		_dir;

		public:
			AxisState2D(int range, int deadZone);
			//! 明示的な初期化(コンストラクタでも呼ばれる)
			void init();
			//! デバッグ用(内部の値が正常かをチェック)
			[[nodiscard]] bool check() const;

			// --- 毎フレーム呼ぶ関数 ---
			//! 前回と入力の変化が無いときに呼ぶ
			void update();
			//! 内部ステートの更新
			void update(int hValue, int vValue);
			// ----------------------------

			[[nodiscard]] const AxisState& axis(Dir dir) const;
			[[nodiscard]] AxisState& refAxis(Dir dir);
			[[nodiscard]] const Vec2& dir() const;

			void setDeadZone(int dz);
	};
}
