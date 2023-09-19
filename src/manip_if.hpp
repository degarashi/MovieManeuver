#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace dg {
	struct Manip;
	using ManipF = void (Manip::*)(HWND) const;

	struct ActionParam {
		virtual void foreground() = 0;
		virtual void callManip(ManipF func) const = 0;
	};
}
