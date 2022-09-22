#pragma once

namespace dg {
    template <class T>
    struct VecBase {
        T x, y;
    };
    using Vec2 = VecBase<float>;
    using IVec2 = VecBase<int>;
}