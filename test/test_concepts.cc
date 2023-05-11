#include <basic_bilinearal.hh>
#include <array>
#include <algorithm>
#include <iostream>

auto main() -> int
{
    // std::cout << cgt::is_linear<cgt::basic_vec_t<int, 4>> << std::endl;
    // std::cout << cgt::is_linear<std::vector<int>> << std::endl;
    // std::cout << cgt::is_dual<cgt::vec_t<int, 2>, cgt::vec_t<int, 2>> << std::endl;
    // std::cout << cgt::is_dual<cgt::vec_t<int, 2>, cgt::vec_t<int, 2, true>> << std::endl;
    cgt::vec2_t<double> a{1, 2};
    cgt::vec2_t<double> b{2, 3};
    std::cout << a * ~b << std::endl;
    std::cout << sizeof(a) << " " << sizeof(b) << std::endl;
    std::cout << a << ", " << b << std::endl;
    return 0;
}