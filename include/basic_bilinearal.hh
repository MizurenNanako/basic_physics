#pragma once
#include <basic_linearal.hh>

namespace cgt
{
    template <typename L, typename R>
    requires is_linear<L> && is_linear<R>
    class basic_bilinearal
    {
    };

} // namespace cgt
