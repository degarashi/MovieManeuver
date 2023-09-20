#include "keyaction.hpp"

namespace dg::input {
	// --- Act_Func ---
	void Act_Func::action(ActionParam& ap) const {
		if(bForeground)
			ap.foreground();
		func(ap);
	}

	// --- Act_Method ---
	Act_Method::Act_Method(ManipF method, bool bForeground):
		Act_Func([method](ActionParam& ap){
			ap.callManip(method);
		}, bForeground) {}
}
