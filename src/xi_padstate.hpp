#pragma once
#include <xinput.h>
#include "vector.hpp"
#include "axisstate_n.hpp"
#include "aux_enums.hpp"
#include "xi_enums.hpp"
#include "virtual_key.hpp"

namespace dg::xinput {
	// XInput用のPadState
	class PadState {
		public:
			const static int TRIGGER_RANGE,
							  THUMB_RANGE;
			const static int DEFAULT_DZ_TRIGGER,
							  DEFAULT_DZ_THUMB;

		private:
			using AxisState2D = AxisState_N<2>;

			ButtonState _button[static_cast<int>(Button::NumButtons)];
			TriggerState _trigger[static_cast<int>(Trigger::NumTrigger)];
			AxisState2D _thumb[static_cast<int>(Thumb::NumThumb)];

			static auto BtnId(const Button id) { return static_cast<int>(id); }
			static auto ThumbId(const Thumb id) { return static_cast<int>(id); }
			static auto TriggerId(const Trigger id) { return static_cast<int>(id); }
			const ButtonState& _thumbAsButtonState(Thumb id, Direction4 dir) const;

		public:
			PadState();
			[[nodiscard]] bool check() const;
			// init: true=counterもリセット
			void setNeutral(bool init);
			// 前回と入力の変化が無いときに呼ぶ
			void updateState();
			void updateState(const XINPUT_GAMEPAD& pad);

			[[nodiscard]] bool pressed(Button id) const;
			[[nodiscard]] bool released(Button id) const;
			[[nodiscard]] ButtonState::Frames pressing(Button id) const;

			[[nodiscard]] const TriggerState& getTrigger(Trigger t) const;
			[[nodiscard]] Vec2 getThumb(Thumb t) const;

			[[nodiscard]] IVec2 getDPadVec() const;

			[[nodiscard]] int getThumbDeadZone(Thumb id) const;
			void setThumbDeadZone(Thumb id, int dz);
			[[nodiscard]] int getTriggerDeadZone(Trigger id) const;
			void setTriggerDeadZone(Trigger id, int dz);

			[[nodiscard]] const AxisState2D& thumb(Thumb id) const;
			[[nodiscard]] AxisState2D& refThumb(Thumb id);

			//! Thumbのpressed()
			[[nodiscard]] bool thumbTilted(Thumb id, Direction4 dir) const;
			//! Thumbのreleased()
			[[nodiscard]] bool thumbRestored(Thumb id, Direction4 dir) const;

			using VKStateAr = std::array<ButtonState, Num_VirtualKey>;
			[[nodiscard]] VKStateAr getState() const;
	};
}
