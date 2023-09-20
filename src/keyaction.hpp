#pragma once
#include "manip_if.hpp"
#include <functional>
#include <memory>

namespace dg::input {
	//! 動作を定義
	struct Action {
		virtual void action(ActionParam& ap) const = 0;
	};
	struct Act_Func : Action {
		using Func = std::function<void (ActionParam&)>;
		Func	func;
		bool	bForeground;

		template <class F>
		Act_Func(F&& f, bool bForeground):
			  func(std::forward<F>(f)),
			  bForeground(bForeground) {}
		void action(ActionParam& ap) const override;
	};
	struct Act_Method : Act_Func {
		ManipF	method;
		Act_Method(ManipF method, bool bForeground);
	};
	using Action_S = std::shared_ptr<Action>;
}
