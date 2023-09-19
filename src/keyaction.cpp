#include "keyaction.hpp"

namespace dg::input {
	// --- Act_Method ---
	Act_Method::Act_Method(ManipF method, bool bForeground):
		  method(method), bForeground(bForeground){}

	void Act_Method::action(ActionParam& ap) const {
		if(bForeground)
			ap.foreground();
		ap.callManip(method);
	}
}
