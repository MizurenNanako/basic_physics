#pragma once
#include <concepts>
#include <functional>

namespace cgt
{
    template <unsigned N, typename ArrayLike>
    constexpr auto with_size(ArrayLike arr) -> bool
    {
        return arr.size() == N;
    }

} // namespace cgt
