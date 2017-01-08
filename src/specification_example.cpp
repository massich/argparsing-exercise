#include <iostream>
#include <string>

template <typename T>
struct Foo
{
private:
  T val;
public:
  Foo( const T &val_): val(val_) {}
  T get(){ return val; }
  void print()
  { std::cout << "this is the template, and the value is " << val << std::endl;}
};

template <>
struct Foo<std::string>
{
private:
  std::string val;
public:
  Foo( const std::string &val_): val(val_) {}
  std::string get(){ return val; }
  void print()
  { std::cout << "this is the specification, and the value is " << val << std::endl;}
};

int main()
{
  Foo<int> foo(3);
  foo.print();
  Foo<std::string> bar("Bar");
  bar.print();
  return 0;
}
