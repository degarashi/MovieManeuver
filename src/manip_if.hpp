#pragma once

namespace dg {
	struct ManipParam;
	struct Manip;
	using ManipF = void (Manip::*)(const ManipParam&) const;
	struct ActionParam {
		virtual void foreground() = 0;
		virtual void callManip(ManipF method, int param=0) const = 0;
	};
}
