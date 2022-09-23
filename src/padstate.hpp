#pragma once
#include <windows.h>
#include <xinput.h>
#include "vector.hpp"

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
	class PadState {
		public:
			enum E_Button {
				Start,
				Back,
				A,B,X,Y,
				LeftThumb, LeftShoulder,
				RightThumb, RightShoulder,
				DPadLeft, DPadUp, DPadRight, DPadDown,

				NumButtons,
				LeftThumbPX = NumButtons,
				LeftThumbNX,
				LeftThumbPY,
				LeftThumbNY,
				RightThumbPX,
				RightThumbNX,
				RightThumbPY,
				RightThumbNY,

				LeftTrigger,
				RightTrigger,
				NumButtonsAll
			};
			enum E_Thumb {
				ThumbLeft,
				ThumbRight,
				NumThumb
			};
			enum E_Trigger {
				TriggerLeft,
				TriggerRight,
				NumTrigger
			};
			const static int TRIGGER_RANGE,
								THUMB_RANGE;
			const static int DEFAULT_DZ_TRIGGER,
								DEFAULT_DZ_THUMB;

		private:
			ButtonState _button[E_Button::NumButtonsAll];
			// 0.0 -> 1.0
			float _trigger[E_Trigger::NumTrigger];
			// -1.0 -> 1.0
			Vec2    _thumb[E_Thumb::NumThumb];

			struct DeadZone {
				int thumb[E_Thumb::NumThumb] = {
					DEFAULT_DZ_THUMB,
					DEFAULT_DZ_THUMB,
				};
				int trigger[E_Trigger::NumTrigger] = {
					DEFAULT_DZ_TRIGGER,
					DEFAULT_DZ_TRIGGER,
				};
			} _deadzone;

			void _updateButton(int idx, bool prev, bool cur);

        public:
            PadState();
            [[nodiscard]] bool check() const;
            // init: true=counterもリセット
            void setNeutral(bool init);
            // 前回と入力の変化が無いときに呼ぶ
            void updateState();
            void updateState(const XINPUT_GAMEPAD& pad);

            [[nodiscard]] bool pressed(E_Button id) const;
            [[nodiscard]] bool released(E_Button id) const;
			[[nodiscard]] ButtonState::Frames pressing(E_Button id) const;

            [[nodiscard]] float getTrigger(E_Trigger t) const;
            [[nodiscard]] Vec2 getThumb(E_Thumb t) const;

            [[nodiscard]] IVec2 getDPadVec() const;

			[[nodiscard]] int getThumbDeadZone(E_Thumb id) const;
			void setThumbDeadZone(E_Thumb id, int dz);
			[[nodiscard]] int getTriggerDeadZone(E_Trigger id) const;
			void setTriggerDeadZone(E_Trigger id, int dz);
	};
}
