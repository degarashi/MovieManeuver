#pragma once
#include "manip_if.hpp"
#include <memory>

namespace dg::input {
	//! 動作を定義
	struct Action {
		virtual void action(ActionParam& ap) const = 0;
	};
	struct Act_Method : Action {
		ManipF	method;
		bool	bForeground;

		Act_Method(ManipF method, bool bForeground);
		void action(ActionParam& ap) const override;
	};
	using Action_S = std::shared_ptr<Action>;
}
