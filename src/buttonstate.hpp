#pragma once

namespace dg {
	//! ボタンPress, Release, Pressing時間の計測
	class ButtonState {
		public:
			using Frames = int;
		private:
			//! 押してるフレーム数カウント (-1 = 離した直後のフレーム)
			Frames	_counter;
			//! 現在ボタンが押されているか
			bool	_state;

		public:
			ButtonState();
			//! 明示的な初期化（コンストラクタでも呼ばれる）
			void init();
			//! 次のボタンステートを入力して内部のカウンタを更新
			void update(bool current);
			//! 前回のupdateから変化が無い時に呼ぶ
			void update();
			//! デバッグ用。内部のカウンタ値が正常かを判定
			bool check() const;

			// ---- 現在の状態を取得 ----
			//! たった今押された
			[[nodiscard]] bool pressed() const;
			//! たった今離された
			[[nodiscard]] bool released() const;
			//! 現在、押されている
			[[nodiscard]] Frames pressing() const;
	};
}
