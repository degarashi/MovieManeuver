#pragma once

namespace dg {
	template <class T>
	bool IsInRange(const T& val, const T& min, const T& max) {
		return (val >= min) && (val <= max);
	}
}
