#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace dg {
	struct Manip;
	struct ManipParam {
		HWND			hw;
		const Manip*	manip;
		int				aux0;
	};
}
