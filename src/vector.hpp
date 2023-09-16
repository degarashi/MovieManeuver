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

	template <class T>
	struct VecBase<T, 3> {
		union {
			struct {
				T	x, y, z;
			};
			std::array<T,3>		ar;
		};
	};
	using Vec3 = VecBase<float, 3>;
	using IVec3 = VecBase<int, 3>;
}
