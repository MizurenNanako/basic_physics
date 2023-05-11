#pragma once
#include <utility.hh>

namespace cgt
{
    template <typename T>
    concept is_scalar = std::is_arithmetic_v<T>;

    template <typename T>
    concept is_linear = is_scalar<T> || requires(T x) {
        typename T::value_type;
        {
            x + x
        } -> std::convertible_to<T>;
        {
            x - x
        } -> std::convertible_to<T>;
        {
            x *typename T::value_type(0)
        } -> std::same_as<T>;
    };

    template <typename L, typename R>
    concept is_dual = requires(L l, R r) {
        typename L::value_type;
        typename R::value_type;
        {
            l *r
        } -> std::same_as<typename L::value_type>;
    };
} // namespace cgt

namespace cgt
{
    template <typename T, unsigned N, bool dual = false>
    class basic_vec_t
    {
    public:
        friend class basic_vec_t<T, N, !dual>;
        using value_type = T;
        using size_type = size_t;

    public:
        basic_vec_t() = default;
        basic_vec_t(std::initializer_list<T> &&_init) = delete;
        basic_vec_t(std::ranges::contiguous_range auto &&_init) = delete;
        ~basic_vec_t() = default;
        auto operator[](size_type index) -> T & = delete;
        auto operator[](size_type index) const -> const T & = delete;
        auto operator+(const basic_vec_t<T, N, dual> &rhs) const -> basic_vec_t<T, N, dual> = delete;
        auto operator+=(const basic_vec_t<T, N, dual> &rhs) -> basic_vec_t<T, N, dual> & = delete;
        auto operator-(const basic_vec_t<T, N, dual> &rhs) const -> basic_vec_t<T, N, dual> = delete;
        auto operator-=(const basic_vec_t<T, N, dual> &rhs) -> basic_vec_t<T, N, dual> & = delete;
        auto operator*(const T &rhs) const -> basic_vec_t<T, N, dual> = delete;
        auto operator*=(const T &rhs) -> basic_vec_t<T, N, dual> & = delete;
        auto operator*(const basic_vec_t<T, N, !dual> &rhs) const -> T = delete;
        auto operator~() const -> basic_vec_t<T, N, !dual> = delete;
    };
} // namespace cgt

namespace cgt
{
    template <typename T, unsigned N, bool dual = false>
    class vec_t : private basic_vec_t<T, N, dual>
    {
    public:
        using typename basic_vec_t<T, N, dual>::value_type;
        using typename basic_vec_t<T, N, dual>::size_type;

    private:
        std::array<T, N> elems;

    public:
        vec_t(std::initializer_list<T> &&_init)
        {
            std::copy(_init.begin(), _init.end(), elems.begin());
        }
        vec_t(std::ranges::contiguous_range auto &&_init)
        {
            std::copy(_init.begin(), _init.end(), elems.begin());
        }
        ~vec_t() = default;
        auto operator[](size_type index) -> T & { return elems[index]; }
        auto operator[](size_type index) const -> const T & { return elems[index]; }
        auto operator+(const vec_t<T, N, dual> &rhs) const -> vec_t<T, N, dual>
        {
            std::array<T, N> tmp;
            size_t n = 0;
            std::generate(tmp.begin(), tmp.end(),
                          [&n, this, &rhs]() mutable
                          {
                              size_t m = n++;
                              return elems[m] + rhs[m];
                          });
            return vec_t<T, N, dual>{std::move(tmp)};
        }
        auto operator+=(const vec_t<T, N, dual> &rhs) -> vec_t<T, N, dual> &
        {
            size_t n = 0;
            std::for_each_n(elems.begin(), N,
                            [&n, &rhs](T &x) mutable
                            { x += rhs[n++]; });
        }
        auto operator-(const vec_t<T, N, dual> &rhs) const -> vec_t<T, N, dual>
        {
            std::array<T, N> tmp;
            size_t n = 0;
            std::generate(tmp.begin(), tmp.end(),
                          [&n, this, &rhs]() mutable
                          {
                              size_t m = n++;
                              return elems[m] - rhs[m];
                          });
            return vec_t<T, N, dual>{std::move(tmp)};
        }
        auto operator-=(const vec_t<T, N, dual> &rhs) -> vec_t<T, N, dual> &
        {
            size_t n = 0;
            std::for_each_n(elems.begin(), N,
                            [&n, &rhs](T &x) mutable
                            { x -= rhs[n++]; });
        }
        auto operator*(const T &rhs) const -> vec_t<T, N, dual>
        {
            std::array<T, N> tmp;
            size_t n = 0;
            std::generate(tmp.begin(), tmp.end(),
                          [&n, this, &rhs]() mutable
                          {
                              size_t m = n++;
                              return elems[m] * rhs;
                          });
            return vec_t<T, N, dual>{std::move(tmp)};
        }
        auto operator*=(const T &rhs) -> vec_t<T, N, dual> &
        {
            size_t n = 0;
            std::for_each_n(elems.begin(), N,
                            [&n, &rhs](T &x) mutable
                            { x *= rhs; });
        }
        auto operator*(const vec_t<T, N, !dual> &rhs) const -> T
        {
            T res{};
            size_t n = 0;
            std::for_each_n(elems.begin(), N,
                            [&n, &res, &rhs](const T &x) mutable
                            { res += x * rhs[n++]; });
            return res;
        }
        auto operator~() const -> vec_t<T, N, !dual>
        {
            std::array<T, N> tmp;
            std::copy(elems.begin(), elems.end(), tmp.begin());
            return vec_t<T, N, !dual>{std::move(tmp)};
        }
    };
} // namespace cgt

namespace cgt
{
    template <typename T, bool dual>
    class vec_t<T, 2, dual> : private basic_vec_t<T, 2, dual>
    {
    public:
        using typename basic_vec_t<T, 2, dual>::value_type;
        using typename basic_vec_t<T, 2, dual>::size_type;

    private:
        friend class vec_t<T, 2, !dual>;
        T x, y;

    public:
        vec_t(T _x, T _y) : x{_x}, y{_y} {}
        vec_t(std::convertible_to<std::array<T, 2>> auto &&_init) : x{_init[0]}, y{_init[1]} {}
        ~vec_t() = default;
        auto operator[](size_type index) -> T & { return index == 0   ? x
                                                         : index == 1 ? y
                                                                      : 0; }
        auto operator[](size_type index) const -> const T & { return index == 0   ? x
                                                                     : index == 1 ? y
                                                                                  : 0; }
        auto operator+(const vec_t<T, 2, dual> &rhs) const -> vec_t<T, 2, dual> { return vec_t<T, 2, dual>(x + rhs.x, y + rhs.y); }
        auto operator+=(const vec_t<T, 2, dual> &rhs) -> vec_t<T, 2, dual> &
        {
            x += rhs.x;
            y += rhs.y;
        }
        auto operator-(const vec_t<T, 2, dual> &rhs) const -> vec_t<T, 2, dual> { return vec_t<T, 2, dual>(x - rhs.x, y - rhs.y); }
        auto operator-=(const vec_t<T, 2, dual> &rhs) -> vec_t<T, 2, dual> &
        {
            x -= rhs.x;
            y -= rhs.y;
        }
        auto operator*(const T &rhs) const -> vec_t<T, 2, dual> { return vec_t<T, 2, dual>(x * rhs, y * rhs); }
        auto operator*=(const T &rhs) -> vec_t<T, 2, dual> &
        {
            x *= rhs;
            y *= rhs;
        }
        auto operator*(const vec_t<T, 2, !dual> &rhs) const -> T { return x * rhs.x + y * rhs.y; }
        auto operator~() const -> vec_t<T, 2, !dual> { return vec_t<T, 2, !dual>(x, y); }
    };
} // namespace cgt

namespace cgt
{
    template <typename T, bool dual>
    class vec_t<T, 3, dual> : private basic_vec_t<T, 3, dual>
    {
    public:
        using typename basic_vec_t<T, 3, dual>::value_type;
        using typename basic_vec_t<T, 3, dual>::size_type;

    private:
        friend class vec_t<T, 3, !dual>;
        T x, y, z;

    public:
        vec_t(T _x, T _y, T _z) : x{_x}, y{_y}, z{_z} {}
        vec_t(std::convertible_to<std::array<T, 3>> auto &&_init) : x{_init[0]}, y{_init[1]}, z{_init[2]} {}
        ~vec_t() = default;
        auto operator[](size_type index) -> T & { return index == 0   ? x
                                                         : index == 1 ? y
                                                         : index == 2 ? z
                                                                      : 0; }
        auto operator[](size_type index) const -> const T & { return index == 0   ? x
                                                                     : index == 1 ? y
                                                                     : index == 2 ? z
                                                                                  : 0; }
        auto operator+(const vec_t<T, 3, dual> &rhs) const -> vec_t<T, 3, dual> { return vec_t<T, 3, dual>(x + rhs.x, y + rhs.y, z + rhs.z); }
        auto operator+=(const vec_t<T, 3, dual> &rhs) -> vec_t<T, 3, dual> &
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
        }
        auto operator-(const vec_t<T, 3, dual> &rhs) const -> vec_t<T, 3, dual> { return vec_t<T, 3, dual>(x - rhs.x, y - rhs.y, z - rhs.z); }
        auto operator-=(const vec_t<T, 3, dual> &rhs) -> vec_t<T, 3, dual> &
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
        }
        auto operator*(const T &rhs) const -> vec_t<T, 3, dual> { return vec_t<T, 3, dual>(x * rhs, y * rhs, z * rhs); }
        auto operator*=(const T &rhs) -> vec_t<T, 3, dual> &
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;
        }
        auto operator*(const vec_t<T, 3, !dual> &rhs) const -> T { return x * rhs.x + y * rhs.y + z * rhs.z; }
        auto operator~() const -> vec_t<T, 3, !dual> { return vec_t<T, 3, !dual>(x, y, z); }
    };
} // namespace cgt

namespace cgt
{
    template <typename T>
    using vec2_t = vec_t<T, 2, false>;
    template <typename T>
    using vec3_t = vec_t<T, 3, false>;
} // namespace cgt
