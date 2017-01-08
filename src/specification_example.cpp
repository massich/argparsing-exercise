#include <iostream>

void print_hi() { std::cout << "hi" << std::endl;}
void print_foo() { std::cout << "foo" << std::endl;}

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

int main()
{
  Foo<int> foo(3);
  foo.print();
  return 0;
}
