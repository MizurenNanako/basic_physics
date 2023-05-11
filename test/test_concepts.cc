#include <basic_bilinearal.hh>
#include <array>
#include <algorithm>
#include <iostream>

auto main() -> int
{
    std::cout << cgt::is_linear<cgt::basic_vec_t<int, 4>> << std::endl;
    std::cout << cgt::is_linear<std::vector<int>> << std::endl;
    std::cout << cgt::is_dual<cgt::vec_t<int, 2>, cgt::vec_t<int, 2>> << std::endl;
    std::cout << cgt::is_dual<cgt::vec_t<int, 2>, cgt::vec_t<int, 2, true>> << std::endl;
    cgt::vec_t<int, 5> a({1, 2, 3, 4, 5});
    cgt::vec_t<int, 5> b({3, 4, 6, 7, 8});
    std::cout << a * ~b << std::endl;
    std::cout << sizeof(a) << " " << sizeof(b) << std::endl;
    return 0;
}