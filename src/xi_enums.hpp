#pragma once

namespace dg::xinput {
	enum class Button {
		Start,
		Back,
		A,B,X,Y,
		LeftThumb, LeftShoulder,
		RightThumb, RightShoulder,
		DPadLeft, DPadUp, DPadRight, DPadDown,

		NumButtons
	};
	enum class Thumb {
		ThumbLeft,
		ThumbRight,
		NumThumb
	};
	enum class Trigger {
		TriggerLeft,
		TriggerRight,
		NumTrigger
	};
}
