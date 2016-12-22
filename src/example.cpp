#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>

template < typename T, std::size_t N>
struct S1{
  std::array<T,N> val;
};

template < typename T, std::size_t N>
struct S2{
  std::array<T,N> val;
  operator std::array<T,N>&() { std::cout << "hi1" << std::endl; return val; }
  operator const std::array<T,N>&() const { std::cout << "hi1" << std::endl; return val; }
};

template < typename T, std::size_t N>
struct S3{
  T val;
  operator S3&() { return val; }
  operator const S3&() const { return val;}
};

template < typename T, std::size_t N>
using s4 = std::array<T,N>;

int main()
{
    // construction uses aggregate initialization
    S1<int, 3> a1{ {1, 2, 3} }; // double-braces required in C++11 (not in C++14)
    S2<int, 3> a4{ {1, 2, 3} }; // double-braces required in C++11 (not in C++14)
    std::array<int, 3> a2 = {1, 2, 3};  // never required after =
    std::array<std::string, 2> a3 = { std::string("a"), "b" };

    // container operations are supported
    std::sort(a1.val.begin(), a1.val.end());
    std::reverse_copy(a2.begin(), a2.end(),
                      std::ostream_iterator<int>(std::cout, " "));

    std::cout << '\n';

    // ranged for loop is supported
    for (const auto& a: a1.val)
        std::cout << a << ' ';

    std::cout << "---------" << '\n';

    // ranged for loop is supported
//    for (const auto& a: a4)
//        std::cout << a << ' ';
    std::reverse_copy(a4.val.begin(), a4.val.end(),
                      std::ostream_iterator<int>(std::cout, " "));
    s4<int,3> a5 { {5,6,7}};

    std::reverse_copy(a5.begin(),a5.end(),
                      std::ostream_iterator<int>(std::cout, " "));
}
