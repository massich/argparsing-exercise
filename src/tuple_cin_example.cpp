#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>
#include <tuple>
#include <istream>
#include <ostream>
#include <sstream>


struct MyType{
  int i;
  std::string s;
};

bool operator==(const MyType &a, const MyType &b)
{
  return std::tie(a.i, a.s) == std::tie(b.i, b.s);
}

std::ostream& operator << (std::ostream& o, const MyType &element)
{
  return o << element.i << ", " << element.s ;
}

std::istream& operator>>(std::istream &input,MyType &o)
{
  return input >> o.i >> o.s;
}

using MyTuple = std::tuple<int, std::string>;
std::istream& operator>>(std::istream& is, MyTuple& obj)
{
  int i;
  std::string s;

  is >> i >> s;
  obj = std::make_tuple(i, s);

  return is;
}

std::ostream& operator<<(std::ostream& os, const MyTuple& obj)
{
  return os << std::get<0>(obj) <<", " << std::get<1>(obj);
}

int main()
{
  MyType xx = {42, "foo"};
  std::cout << "initial value : " << xx <<  std::endl;
  std::stringstream("0 xxxx") >> xx;
  std::cout << "parsed value : " << xx <<  std::endl;

  MyTuple yy;
  std::stringstream("24 yyyy") >> yy;
  std::cout << "initial value : " << yy <<  std::endl;

  return 0;
}
