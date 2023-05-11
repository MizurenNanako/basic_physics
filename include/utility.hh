#pragma once
#include <concepts>
#include <functional>
#include <ostream>

namespace cgt
{
    template <unsigned N, typename ArrayLike>
    constexpr auto with_size(ArrayLike arr) -> bool
    {
        return arr.size() == N;
    }

    template <unsigned S, unsigned E>
        requires(S < E)
    class integral_span
    {
    public:
        class iterator
        {
        public:
            using iterator_category = std::contiguous_iterator_tag;
            using value_type = size_t;
            using difference_type = size_t;
            using pointer = const size_t *;
            using reference = size_t;

        private:
            unsigned num;

        public:
            explicit iterator(unsigned _num = 0) : num(_num) {}
            auto operator++() -> iterator &
            {
                ++num;
                return *this;
            }
            auto operator++(int) -> iterator { return *this, ++num; }
            auto operator+(size_t rhs) -> iterator { return iterator(num + rhs); }
            bool operator==(iterator other) const { return num == other.num; }
            bool operator!=(iterator other) const { return !(*this == other); }
            reference operator*() const { return num; }
        };
        auto begin() -> iterator { return iterator(S); }
        auto end() -> iterator { return iterator(E); }
    };

} // namespace cgt
