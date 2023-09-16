#pragma once
#include <cstddef>
#include <array>

namespace dg {
	template <class T, size_t N>
	struct VecBase;

	template <class T>
	struct VecBase<T, 2> {
		union {
			struct {
				T	x, y;
			};
			std::array<T,2>		ar;
		};
	};

	using Vec2 = VecBase<float, 2>;
	using IVec2 = VecBase<int, 2>;
}
