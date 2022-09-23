#pragma once

namespace dg {
	class ButtonState {
		public:
			using Frames = int;
		private:
			// 押してるフレーム数カウント (-1 = 離した直後のフレーム)
			Frames	_counter;
			bool	_state;

		public:
			ButtonState();
			void init();
			void update(bool current);
			void update();
			bool check() const;
			[[nodiscard]] bool pressed() const;
			[[nodiscard]] bool released() const;
			[[nodiscard]] Frames pressing() const;
	};
}
