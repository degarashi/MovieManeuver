#pragma once
#include <windows.h>
#include <xinput.h>
#include "vector.hpp"
#include "axisstate2d.hpp"

namespace dg {
	// XInput用のPadState
	class XI_PadState {
		public:
			enum E_Button {
				Start,
				Back,
				A,B,X,Y,
				LeftThumb, LeftShoulder,
				RightThumb, RightShoulder,
				DPadLeft, DPadUp, DPadRight, DPadDown,

				NumButtons
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
			ButtonState _button[E_Button::NumButtons];
			TriggerState _trigger[E_Trigger::NumTrigger];
			AxisState2D _axis[E_Thumb::NumThumb];

			void _updateButton(int idx, bool prev, bool cur);

		public:
			XI_PadState();
			[[nodiscard]] bool check() const;
			// init: true=counterもリセット
			void setNeutral(bool init);
			// 前回と入力の変化が無いときに呼ぶ
			void updateState();
			void updateState(const XINPUT_GAMEPAD& pad);

			[[nodiscard]] bool pressed(E_Button id) const;
			[[nodiscard]] bool released(E_Button id) const;
			[[nodiscard]] ButtonState::Frames pressing(E_Button id) const;

			[[nodiscard]] const TriggerState& getTrigger(E_Trigger t) const;
			[[nodiscard]] Vec2 getThumb(E_Thumb t) const;

			[[nodiscard]] IVec2 getDPadVec() const;

			[[nodiscard]] int getThumbDeadZone(E_Thumb id) const;
			void setThumbDeadZone(E_Thumb id, int dz);
			[[nodiscard]] int getTriggerDeadZone(E_Trigger id) const;
			void setTriggerDeadZone(E_Trigger id, int dz);

			[[nodiscard]] const AxisState2D& axis(E_Thumb id) const;
			[[nodiscard]] AxisState2D& refAxis(E_Thumb id);
	};
}
